#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "BUFFALO-G-0AD8";
const char* password = "r6ju8tjjtnup5";
String deviceID = "bedroom";
WiFiClient client;

void setup() {
  Serial.begin(115200);
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
  
  // Post data to dweet.io
  HTTPClient http;
  String URL = "http://dweet.io/dweet/for/" + deviceID + "?foo=bar&test=123"; // Works with HTTP
  http.begin(client,URL); // Works with HTTP
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload); // Print response
  }
  
  http.end();
}

void loop() {
  // Nothing here
}
