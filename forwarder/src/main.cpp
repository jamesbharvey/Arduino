#include <Arduino.h>

#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "wifi_secrets.h"

const char* ssid = WIFISSID;
const char* password = WIFIPASSWORD;
WiFiClient client;
#define UDP_PORT 2004 //graphite udp is 2003, we use 2004
WiFiUDP Udp;

IPAddress destinationIp(192, 168, 11, 23);


/*
 void httpFetch(String inString) {

  HTTPClient http;
  String URL = "http://192.168.11.23/sensorLog?" + inString;
  http.begin(client,URL); 
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println(URL);
    String payload = http.getString();
    Serial.println(payload); 
  }
  
  http.end();

}  
*/

void sendUDP(String inString) {
  Serial.println("Sending UDP message " + inString);
  Udp.beginPacket(destinationIp, UDP_PORT);
  Udp.write(inString.c_str());
  Udp.write('\n');
  Udp.endPacket();
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  const char delimeter = '\n';
  String inString = Serial.readStringUntil(delimeter);
  if (inString == "") {
    Serial.println("no input");
    return;
  }
  sendUDP(inString);
}




