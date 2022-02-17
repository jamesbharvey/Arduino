#include <Arduino.h>
#include <Wire.h>
#include "DS1307.h"
//#include <U8g2lib.h>
#include <U8x8lib.h>

//const unsigned char temp_bmp1[] U8X8_PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x07, 0xfe, 0xff, 0xff, 0x1f, 0x8f, 0xff, 0xff, 0x3f, 0x0f, 0x00, 0x00, 0x3e, 0x0f, 0x00, 0x00, 0x3e, 0x8f, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x07, 0xfc, 0x01, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

DS1307 clock;//define a object of DS1307 class
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE);
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup()
{
    Serial.begin(9600);
    clock.begin();
//    clock.fillByYMD(2022,2,16);
//    clock.fillByHMS(15,16,10);
//    clock.fillDayOfWeek(WED);
//    clock.setTime();//write time to the RTC chip
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);
}
void loop()
{
    printTime();

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

//    Serial.print(clock.minute, DEC);
//    Serial.print(":");
//    Serial.print(clock.second, DEC);
//    Serial.print("  ");
//    Serial.print(clock.month, DEC);
//    Serial.print("/");
//    Serial.print(clock.dayOfMonth, DEC);
//    Serial.print("/");
//    Serial.print(clock.year+2000, DEC);

    displayTimeU8x8(theTime); 
 //   displayTime();
 //   delay(5000);
}
    /*Function: Display time on the serial monitor*/

void displayTimeU8x8(String theTime)
{
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(0, 33);
    u8x8.print(theTime);
}


//void displayTime()
//{
//      u8g2.firstPage();
//      do {
//        u8g2.setFont(u8g2_font_t0_12b_mr);//
//        u8g2.drawXBMP(49, 0, 30, 30, temp_bmp1);
//        u8g2.setCursor(31, 35);
//        u8g2.print(F("Temperature")); // write something to the internal memory
//        u8g2.setCursor(43, 49);
//        u8g2.print(F("Humidity"));    // write something to the internal memory
//      } while (u8g2.nextPage());
//}

void printTime()
{
    clock.getTime();
    Serial.print(clock.hour, DEC);
    Serial.print(":");
    Serial.print(clock.minute, DEC);
    Serial.print(":");
    Serial.print(clock.second, DEC);
    Serial.print("  ");
    Serial.print(clock.month, DEC);
    Serial.print("/");
    Serial.print(clock.dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(clock.year+2000, DEC);
    Serial.print(" ");
    Serial.print(clock.dayOfMonth);
    Serial.print("*");
    switch (clock.dayOfWeek)// Friendly printout the weekday
    {
        case MON:
        Serial.print("MON");
        break;
        case TUE:
        Serial.print("TUE");
        break;
        case WED:
        Serial.print("WED");
        break;
        case THU:
        Serial.print("THU");
        break;
        case FRI:
        Serial.print("FRI");
        break;
        case SAT:
        Serial.print("SAT");
        break;
        case SUN:
        Serial.print("SUN");
        break;
    }
    Serial.println(" ");
}
