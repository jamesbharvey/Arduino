#include "Arduino.h"
#include <Wire.h>
#include <DS1307.h>
#include <U8x8lib.h>

DS1307 clock;//define a object of DS1307 class
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


void setup()
{
  Serial.begin(9600);
  clock.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);
}

void displayTimeU8x8(String theTime)
{
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(0, 33);
    u8x8.print(theTime);
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
    //Serial.println(theTime);
    displayTimeU8x8(theTime); 

}


