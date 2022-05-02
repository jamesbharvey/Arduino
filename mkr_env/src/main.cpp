#include <Arduino.h>

#include <Arduino_MKRENV.h>
#include "wifi_secrets.h"
#include <WiFiNINA.h>
#include <WiFiUdp.h>

char ssid[] = WIFISSID;      // your network SSID (name)
char pass[] = WIFIPASSWORD;  // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS; // the Wifi radio's status
WiFiUDP Udp;
// graphite udp is 2003, we use 2004
#define UDP_PORT 2004

IPAddress destinationIp(192, 168, 11, 23);

void printWifiData()
{
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

void sendDataToGraphite(String tag, float data)
{
  String message = "mysensors.livingroom." + tag + " " + data + " -1";
  Serial.print(destinationIp);
  Serial.print(" ");
  Serial.print(UDP_PORT);
  Serial.print(" ");
  Serial.println(message);

  Udp.beginPacket(destinationIp, UDP_PORT);
  Udp.write(message.c_str());
  Udp.write('\n');
  Udp.endPacket();
  delay(100);
}

void setup()
{

  Serial.begin(9600);

  //while (!Serial)
  //  ;
  delay(1000);

  if (!ENV.begin())
  {

    Serial.println("Failed to initialize MKR ENV shield!");

    while (1)
      ;
  }

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }


  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  Serial.println("----------------------------------------");
  printWifiData();
  Serial.println("----------------------------------------");

  Udp.begin(UDP_PORT);
}

void loop()
{

  float temperature = ENV.readTemperature();
  sendDataToGraphite("temperature", temperature);

  float humidity = ENV.readHumidity();
  sendDataToGraphite("humidity", humidity);

  float pressure = ENV.readPressure();
  sendDataToGraphite("pressure", pressure);

  float illuminance = ENV.readIlluminance();
  sendDataToGraphite("illuminance", illuminance);

  float uva = ENV.readUVA();
  sendDataToGraphite("uva", uva);

  float uvb = ENV.readUVB();
  sendDataToGraphite("uvb", uvb);

  float uvIndex = ENV.readUVIndex();
  sendDataToGraphite("uvIndex", uvIndex);

  Serial.print("Temperature = ");

  Serial.print(temperature);

  Serial.println(" C");

  Serial.print("Humidity    = ");

  Serial.print(humidity);

  Serial.println(" %");

  Serial.print("Pressure    = ");

  Serial.print(pressure);

  Serial.println(" kPa");

  Serial.print("Illuminance = ");

  Serial.print(illuminance);

  Serial.println(" lx");

  Serial.print("UVA         = ");

  Serial.println(uva);

  Serial.print("UVB         = ");

  Serial.println(uvb);

  Serial.print("UV Index    = ");

  Serial.println(uvIndex);

  Serial.println();

  delay(30000);

  printWifiData();
  
  Serial.println("----------------------------------------");
}