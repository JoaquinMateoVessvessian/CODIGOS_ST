#include <DHT.h>
#include <DHT_U.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

#include <Adafruit_Sensor.h>
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
  if(t>28){
    digitalWrite(26,HIGH);
  }
  else{
    digitalWrite(26,LOW);
  }
  sprintf(stringt, "%.2f", t);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(15, 15, stringt);
  u8g2.sendBuffer();
  delay(100);
}
