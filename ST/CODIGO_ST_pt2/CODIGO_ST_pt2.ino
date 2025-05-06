#include <DHT.h>
#include <DHT_U.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

#include <Adafruit_Sensor.h>
#define DHTTYPE DHT11
#define DHTPIN 23
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
DHT dht(DHTPIN, DHTTYPE);
float t;
char stringt[6];
void setup() {
  Serial.begin(9600);
  Serial.println(F("OLED test"));
  u8g2.begin();
  dht.begin();
}

void loop() {
  t=dht.readTemperature();
  sprintf(stringt, "%.2f", t);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(15, 15, stringt);
  u8g2.sendBuffer();
  delay(100);
}
