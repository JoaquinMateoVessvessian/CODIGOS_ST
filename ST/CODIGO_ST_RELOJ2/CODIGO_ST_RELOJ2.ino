// VESSVESSIAN, BAIRROS, KANG, SAGGIORATTO

#include <WiFi.h>
#include <ESP32Time.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 23
#define DHTTYPE DHT11
#define P1 200
#define P2 201
#define ESPERA1 202
#define ESPERA2 203
#define SUMAGMT 204
#define RESTAGMT 205
#define BOTON1 35
#define BOTON2 34

const char *ssid = "ORT-IoT";
const char *password = "NuevaIOT$25";
const char *ntpServer = "pool.ntp.org";

int boton1, boton2;
bool resta, suma;
int gmt = 0;
int estado;

ESP32Time rtc(gmt);
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset = */ U8X8_PIN_NONE);

void Maquina(float t, int EstadoBoton1, int EstadoBoton2);

void setup() {
  Serial.begin(9600);
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);

  u8g2.begin();
  dht.begin();

  WiFi.begin(ssid, password);
  delay(100);

  if (WiFi.status() == WL_CONNECTED) {
    configTime(0, 0, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      rtc.setTimeStruct(timeinfo);
    } 

  suma = true;
  resta = true;
  estado = P1;
}

}
void loop() {
  float t = dht.readTemperature();
  boton1 = digitalRead(BOTON1);
  boton2 = digitalRead(BOTON2);
  Maquina(t, boton1, boton2);
  delay(100);
}

void Maquina(float t, int EstadoBoton1, int EstadoBoton2) {
  char stringt[6];
  char stringtiempo[10];
  char stringgmt[3];
  sprintf(stringt, "%.2f", t);

  rtc = ESP32Time(gmt);
  int hora = rtc.getHour(true);
  int minute = rtc.getMinute();
  int second = rtc.getSecond();
  sprintf(stringtiempo, "%02d:%02d:%02d", hora, minute, second);

  switch (estado) {
    case P1:
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(15, 15, "Temp:");
      u8g2.drawStr(60, 15, stringt);
      u8g2.drawStr(15, 30, "Hora:");
      u8g2.drawStr(60, 30, stringtiempo);
      u8g2.sendBuffer();
      if (EstadoBoton1 == LOW && EstadoBoton2 == LOW) {
        estado = ESPERA1;
      }
      break;

    case ESPERA1:
      if (EstadoBoton1 == HIGH && EstadoBoton2 == HIGH) {
        estado = P2;
      }
      break;
    case ESPERA2:
      if (EstadoBoton1 == HIGH && EstadoBoton2 == HIGH) {
        estado = P1;
      }
      break;
    case P2:
      sprintf(stringgmt, "%d", gmt/3600);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(15, 30, "GMT:");
      u8g2.drawStr(60, 30, stringgmt);
      u8g2.sendBuffer();
      if (EstadoBoton1 == LOW && EstadoBoton2 == LOW) {
        estado = ESPERA2;
      }
      if(EstadoBoton1 == LOW && EstadoBoton1 == HIGH) {
        estado = SUMAGMT;
      } 
      if (EstadoBoton2 == LOW && EstadoBoton1 == HIGH) {
        estado = RESTAGMT;
      }
      break;

    case SUMAGMT:
      if (gmt < 43200) {
        gmt += 3600;
      }
      estado = P2;
      break;

    case RESTAGMT:
      if (gmt > -43200) {
        gmt -= 3600;
      }
      estado = P2;
      break;
  }
}
