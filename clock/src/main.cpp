#include "Arduino.h"
#include <DS1307.h>  // Realtime clock library
#include <U8g2lib.h> //OLED Display library

DS1307 clock; // define a object of DS1307 class
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE);

void setup()
{
  clock.begin();
  u8g2.begin();
}

String getTheTime()
{
  String theTime = String(clock.hour, DEC);
  theTime += ":";
  if (clock.minute < 10)
    theTime += "0";
  theTime += String(clock.minute, DEC);
  theTime += ":";
  if (clock.second < 10)
    theTime += "0";
  theTime += String(clock.second, DEC);
  return theTime;
}

String getDayOfWeek()
{
  String theDayOfWeek;
  switch (clock.dayOfWeek)
  {
  case MON:
    theDayOfWeek = "Monday";
    break;
  case TUE:
    theDayOfWeek = "Tuesday";
    break;
  case WED:
    theDayOfWeek = "Wednesday";
    break;
  case THU:
    theDayOfWeek = "Thursday";
    break;
  case FRI:
    theDayOfWeek = "Friday";
    break;
  case SAT:
    theDayOfWeek = "Saturday";
    break;
  case SUN:
    theDayOfWeek = "Sunday";
    break;
  }
  return theDayOfWeek;
}

String getMonth() {
  String theMonth;
  switch (clock.month) {
    case 1:
      theMonth = "January";
      break;
    case 2:
      theMonth = "February";
      break;
    case 3:
      theMonth = "March";
      break;
    case 4:
      theMonth = "April";
      break;
  }
  return theMonth;
}

void displayTime(String theTime)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(25, 36, theTime.c_str());
  } while (u8g2.nextPage());
}

void displayDate(String dayOfMonth, String month)
{
  u8g2.firstPage();
  String theDate = dayOfMonth + " " + month;
  do
  {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(25, 36, theDate.c_str());
  } while (u8g2.nextPage());
}

void displayDayOfWeek(String dayOfWeek)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(25, 36, dayOfWeek.c_str());
  } while (u8g2.nextPage());
}

void loop()
{
  clock.getTime();
  int mod10 = clock.second % 10;
  if (mod10 < 6)
  {
    displayTime(getTheTime());
  }
  else if (mod10 < 8)
  {
    displayDayOfWeek(getDayOfWeek());
  }
  else
  {
    displayDate(String(clock.dayOfMonth, DEC), getMonth());
  }
}
