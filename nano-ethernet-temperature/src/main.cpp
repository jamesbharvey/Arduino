#include <Arduino.h>
#include<OneWire.h>
#include "DallasTemperature.h"
#include <SPI.h>
#include <Ethernet.h>

// DEADBEEFFEED!
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };





// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 10

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();

  // initialize the Ethernet shield using DHCP:
   if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtaining an IP address");

    // check for Ethernet hardware present
    //if (Ethernet.hardwareStatus() == EthernetNoHardware)
    //  Serial.println("Ethernet shield was not found");

    // check for Ethernet cable
    if (Ethernet.linkStatus() == LinkOFF)
      
    while (true) {
      Serial.println("Ethernet cable is not connected.");
      delay(1000);
    };
  }
  
  Serial.println("start");
  Serial.print("Arduino's IP Address: ");
  Serial.println(Ethernet.localIP());

  // Serial.print("DNS Server's IP Address: ");
  // Serial.println(Ethernet.dnsServerIP());

  // Serial.print("Gateway's IP Address: ");
  // Serial.println(Ethernet.gatewayIP());

  // Serial.print("Network's Subnet Mask: ");
  // Serial.println(Ethernet.subnetMask());
}


void loop() {

 /*  byte data[12];
// 温度データを格納する配列
float celsius;
// 温度（摂氏）を格納する変数

ds.reset();
// バスをリセット
ds.select(addr);
// アドレス指定（複数センサがある場合に識別）
ds.write(0x44, 1);
// 温度変換開始コマンドを送信
delay(750);
// 12bit精度では変換に最大750msかかる

ds.reset();
ds.select(addr);
ds.write(0xBE);
// スクラッチパッド（温度データ格納領域）読み出しコマンド

for (byte i = 0; i < 9; i++) {
data[i] = ds.read();
// 9バイト分のデータを順に読み出す
}

int16_t raw = (data[1] << 8) | data[0];
// 上位バイトと下位バイトを結合
celsius = (float)raw / 16.0;
// 16で割ることで摂氏温度に変換

Serial.print("Temp= ");
Serial.println(celsius);
// 温度をシリアルモニタに出力 */
 Serial.println("Hello world");
  // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  // Fetch the temperature in degrees Celsius for device index:
  float tempC = sensors.getTempCByIndex(0); // the index 0 refers to the first device
  // Fetch the temperature in degrees Fahrenheit for device index:
  float tempF = sensors.getTempFByIndex(0);

  // Print the temperature in Celsius in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.print("C  |  ");

  // Print the temperature in Fahrenheit
  Serial.print(tempF);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("F");
  // Wait 1 second:
  delay(1000);

}

