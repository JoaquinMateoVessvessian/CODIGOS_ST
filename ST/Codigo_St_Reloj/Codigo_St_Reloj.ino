//VESSVESSIAN, BAIRROS, KANG, SAGGIORATTO

#include <ESP32Time.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
void Maquina(float t, int EstadoBoton1, int EstadoBoton2);
#define DHTPIN 23
#define DHTTYPE DHT11
#define P1 200
#define P2 300
#define ESPERA 400
#define SUMAHORA 500
#define SUMAMIN 600
#define BOTON1 35
#define BOTON2 34
int hora;
int minute;
int boton1;
int boton2;
ESP32Time rtc(0);
int estado;
bool PASAJE = false;
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  rtc.setTime(30, 38, 11, 25, 4, 2025);
  Serial.println(F("OLED test"));
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);
  u8g2.begin();
  dht.begin();
  hora = rtc.getHour();
  minute = rtc.getMinute();
  estado = P1;
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
  sprintf(stringt, "%.2f", t);
  switch (estado) {
    case P1:
      sprintf(stringtiempo, "%02d:%02d  ", hora, minute);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.drawStr(15, 15, "Temp:");
      u8g2.drawStr(60, 15, stringt);
      u8g2.drawStr(15, 30, "Hora:");
      u8g2.drawStr(60, 30, stringtiempo);
      u8g2.sendBuffer();
      PASAJE = true;
      if (EstadoBoton1 == LOW && EstadoBoton2 == LOW) {
        estado = ESPERA;
      }
      break;
    case ESPERA:
      if (EstadoBoton1 == HIGH && EstadoBoton2 == HIGH) {
        if (PASAJE == false) {
          estado = P1;
        }
        if (PASAJE == true) {
          estado = P2;
        }
      }
      break;
    case P2:
      sprintf(stringtiempo, "%02d:%02d  ", hora, minute);
      u8g2.clearBuffer();
      u8g2.drawStr(15, 30, "Hora:");
      u8g2.drawStr(60, 30, stringtiempo);
      if (EstadoBoton1 == LOW) {
        estado = SUMAHORA;
      }
      if (EstadoBoton2 == LOW) {
        estado = SUMAMIN;
      }
      u8g2.sendBuffer();
      PASAJE = false;
      if (EstadoBoton1 == LOW && EstadoBoton2 == LOW) {
        estado = ESPERA;
      }
      break;
    case SUMAHORA:
      hora = hora + 1;
      if (hora >= 24) {
        hora = 0;
      }
      estado = P2;
      break;

    case SUMAMIN:
      minute = minute + 1;
      if (minute >= 60) {
        minute = 0;
        hora = hora + 1;
      }
      estado = P2;
      break;
  }
}