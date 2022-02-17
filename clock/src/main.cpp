/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <Wire.h>
#include <DS1307.h>
// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

DS1307 clock;//define a object of DS1307 class


void setup()
{
  Serial.begin(9600);
  clock.begin();
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    clock.getTime();
    String theTime = String(clock.hour, DEC);
    theTime += ":";
    if (clock.minute < 10)
      theTime += "0";
    theTime += String(clock.minute,DEC);
    theTime += ":";
    if (clock.second < 10)
      theTime += "0";
    theTime += String(clock.second,DEC);
    Serial.println(theTime);

}