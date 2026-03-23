#include <Arduino.h>
// #include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "wifi_secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

const char *ssid = WIFISSID;
const char *password = WIFIPASSWORD;
WiFiClient client;
#define UDP_PORT 2004 // graphite udp is 2003, we use 2004
WiFiUDP Udp;

IPAddress destinationIp(192, 168, 11, 23);
unsigned long previous_time = 0;
unsigned long delay_between_attempts = 120000; // two minute delay

// there is no 72x40 constructor in u8g2 hence the 72x40 screen is
// mapped in the middle of the 132x64 pixel buffer of the SSD1306 controller
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);
// int width = 68;
// int height = 62;
// int xOffset = (132 - width) / 2;
// int yOffset = 24; //(64-height)/2;

void setup(void)
{
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(5000);
    Serial.print("...connecting wifi.....");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  sensors.begin();

  delay(1000);
  // u8g2.begin();
  // u8g2.enableUTF8Print();		// enable UTF8 support for the Arduino print()
  // u8g2.setContrast(255);    // set contrast to maximum
  // u8g2.setBusClock(400000); // 400kHz I2C
  // //u8g2.setFont(u8g2_font_ncenB24_tr);
  // //u8g2.setFont(u8x8_font_8x13B_1x2_f);
  // u8g2.setFont(u8g2_font_helvR24_tf);
}

void sendUDP(float temperature)
{
  Serial.println("Sending UDP message");
  Udp.beginPacket(destinationIp, UDP_PORT);
  char whisper_message[50];
  sprintf(whisper_message, "mysensors.livingroom.fishtank.temperature %0.2f -1\n", temperature);
  Udp.write((uint8_t *)whisper_message, strlen(whisper_message));
  Udp.endPacket();
}

void loop(void)
{
  // checking for WIFI connection
  while (WiFi.status() == WL_DISCONNECTED)
  {
    unsigned long current_time = millis();
    if (current_time - previous_time >= delay_between_attempts)
    {
      Serial.print(millis());
      Serial.println("Reconnecting to WIFI network");
      WiFi.disconnect();
      WiFi.reconnect();
      previous_time = current_time;
    }
    else
    {
      delay(delay_between_attempts);
    }
  }

  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  sendUDP(temperature);
  Serial.printf("temp is %f",temperature);

  // u8g2.clearBuffer(); // clear the internal memory
  // // u8g2.drawFrame(xOffset+0, yOffset+0, width, height); //draw a frame around the border
  // u8g2.setCursor(xOffset + 4, yOffset + 30);
  // u8g2.printf("%d°", (int)temperature);
  // u8g2.sendBuffer();
  sleep(60);
}
