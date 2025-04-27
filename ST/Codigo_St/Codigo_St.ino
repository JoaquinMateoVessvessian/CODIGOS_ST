#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void printBMP_OLED(void);
void setup() {
Serial.begin(9600);
Serial.println(F("OLED test"));
u8g2.begin();
dht.begin();
}

void loop() {
  printBMP_OLED();
  delay(2000);
float temperatura=dht.readTemperature();

if(temperatura>=28){
  digitalWrite(IO25,HIGH);
}

}

void printBMP_OLED(void)
{
	  u8g2.clearBuffer();         
    u8g2.setFont(u8g2_font_ncenB14_tr); 
    u8g2.drawStr(15,15,temperatura); 
    u8g2.sendBuffer();         
}
