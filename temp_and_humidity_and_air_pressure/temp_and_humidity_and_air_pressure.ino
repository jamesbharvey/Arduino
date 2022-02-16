#include "Seeed_BMP280.h"
#include "DHT.h"
#include <Wire.h>
#include <Arduino.h>
#include <U8x8lib.h>

BMP280 bmp280;

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  if (!bmp280.init()) {
    Serial.println("BMP280 not connected or broken!");
  }
  dht.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);
}

void loop(void) {
  float DhtTemp, humi, Bmp280Temp, pressure;
  DhtTemp = dht.readTemperature();
  humi = dht.readHumidity();
  Bmp280Temp = bmp280.getTemperature();
  pressure = bmp280.getPressure();
  // put your main code here, to run repeatedly:
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 33);
  u8x8.print("Pressure:");
  u8x8.print(pressure);
  u8x8.print("Pa");
//  u8x8.print("Humidity:");
//  u8x8.print(humi);
//  u8x8.print("%");

  u8x8.setCursor(0, 50);
  u8x8.print("Altitude: ");
  u8x8.print(bmp280.calcAltitude(pressure));
  
//  u8x8.print("DHT Temp:");
//  u8x8.print(DhtTemp);
//  u8x8.print("C");


  //  u8x8.print("BMP Temp:");
  //  u8x8.print(Bmp280Temp);
  //  u8x8.print("C");
  u8x8.refreshDisplay();
  delay(200);
}
