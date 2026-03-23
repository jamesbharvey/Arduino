#include <Arduino.h>
#include<OneWire.h>
#include "DallasTemperature.h"
#include <SPI.h>
#include <UIPEthernet.h>

#define UDP_TX_PACKET_MAX_SIZE 100 //increase UDP size

// DEADBEEFFEED!
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 6

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);


#define UDP_PORT 2004 // graphite udp is 2003, we use 2004
EthernetUDP Udp;

IPAddress destinationIp(192, 168, 11, 23);

void sendUDP(String inString)
{
  Serial.println("Sending UDP message " + inString);
  Udp.beginPacket(destinationIp, UDP_PORT);
  Udp.write(inString.c_str());
  Udp.write('\n');
  Udp.endPacket();
}



void setup() {
  Serial.begin(9600);
  sensors.begin();

  // initialize the Ethernet shield using DHCP:
   if (Ethernet.begin(mac) == 0) {
    while (true) {
      Serial.println("Failed to obtain an IP address");
      delay(1000);
    }
   }
    // check for Ethernet hardware present
    //if (Ethernet.hardwareStatus() == EthernetNoHardware)
    //  Serial.println("Ethernet shield was not found");

    // check for Ethernet cable
    if (Ethernet.linkStatus() == LinkOFF)
      
    while (true) {
      Serial.println("Ethernet cable is not connected.");
      delay(1000);
    };
  
  Serial.println("start");
  Serial.print("Arduino's IP Address: ");
  Serial.println(Ethernet.localIP());

  Serial.print("DNS Server's IP Address: ");
  Serial.println(Ethernet.dnsServerIP());

  Serial.print("Gateway's IP Address: ");
  Serial.println(Ethernet.gatewayIP());

  Serial.print("Network's Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());
}


void loop() {

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

  String graphiteMessage = "mysensors.livingroom.fishtank.temperature ";
  graphiteMessage += String(tempC);
  graphiteMessage += " -1";

  sendUDP(graphiteMessage);

  // Print the temperature in Fahrenheit
  Serial.print(tempF);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("F");
  // Wait 120 second:
  delay(120 * 1000);

}

