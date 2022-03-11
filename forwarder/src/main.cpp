#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "wifi_secrets.h"
const char* ssid = WIFISSID;
const char* password = WIFIPASSWORD;
WiFiClient client;

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
    // Post data to dweet.io
  const char delimeter = '\n';
  String inString = Serial.readStringUntil(delimeter);
  if (inString == "") {
    Serial.println("no input");
    return;
  }
  HTTPClient http;
  String URL = "http://192.168.11.23/sensorLog?" + inString;
  http.begin(client,URL); // Works with HTTP
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println(URL);
    String payload = http.getString();
    Serial.println(payload); // Print response
  }
  
  http.end();
  // Nothing here
  delay(1000);
}


// #include <ESP8266WiFi.h>

// void setup() {
//   Serial.begin(9600);
//   Serial.println(F("\nESP8266 WiFi scan example"));

//   // Set WiFi to station mode
//   WiFi.mode(WIFI_STA);

//   // Disconnect from an AP if it was previously connected
//   WiFi.disconnect();
//   delay(100);
// }

// void loop() {
//   String ssid;
//   int32_t rssi;
//   uint8_t encryptionType;
//   uint8_t* bssid;
//   int32_t channel;
//   bool hidden;
//   int scanResult;

//    Serial.println(F("Starting WiFi scan..."));

//   scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

//   if (scanResult == 0) {
//     Serial.println(F("No networks found"));
//   } else if (scanResult > 0) {
//     Serial.printf(PSTR("%d networks found:\n"), scanResult);

//     // Print unsorted scan results
//     for (int8_t i = 0; i < scanResult; i++) {
//       WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

//       Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n"),
//                     i,
//                     channel,
//                     bssid[0], bssid[1], bssid[2],
//                     bssid[3], bssid[4], bssid[5],
//                     rssi,
//                     (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
//                     hidden ? 'H' : 'V',
//                     ssid.c_str());
//       delay(0);
//     }
//   } else {
//     Serial.printf(PSTR("WiFi scan error %d"), scanResult);
//   }
//   // Wait a bit before scanning again
//   Serial.print("Hello");
//   delay(5000);
// }