#include "Arduino.h"
#include <DS1307.h>  // Realtime clock library
#include <U8g2lib.h> //OLED Display library
#include "music.h"

DS1307 clock; // define a object of DS1307 class
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE);

// pins
char led = 4;
char buzzer = 5;
char button = 6;
char rotary = A0;

// hardcoded default alarm time
int alarmHour = 8;
int alarmMinute = 0;

#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t.c_str()))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t.c_str()))
#define ALIGN_LEFT 0

void setup()
{
  clock.begin();
  u8g2.begin();
  pinMode(rotary, INPUT);
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  tuneLength = sizeof(tune) / sizeof(tune[0]);
}

String getTheTime()
{
  String theTime = String(clock.hour, DEC);
  theTime += ":";
  if (clock.minute < 10)
    theTime += "0";
  theTime += String(clock.minute, DEC);
  // theTime += ":";
  // if (clock.second < 10)
  //   theTime += "0";
  // theTime += String(clock.second, DEC);
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

String getMonth()
{
  String theMonth;
  switch (clock.month)
  {
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
  case 5:
    theMonth = "May";
    break;
  case 6:
    theMonth = "June";
    break;
  case 7:
    theMonth = "July";
    break;
  case 8:
    theMonth = "August";
    break;
  case 9:
    theMonth = "September";
    break;
  case 10:
    theMonth = "October";
    break;
  case 11:
    theMonth = "November";
    break;
  case 12:
    theMonth = "December";
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
    u8g2.drawStr(ALIGN_CENTER(theTime), 36, theTime.c_str());
  } while (u8g2.nextPage());
}

void displayDate(String dayOfMonth, String month)
{
  u8g2.firstPage();
  String theDate = dayOfMonth + " " + month;
  do
  {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(ALIGN_CENTER(theDate), 36, theDate.c_str());
  } while (u8g2.nextPage());
}

void displayDayOfWeek(String dayOfWeek)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(ALIGN_CENTER(dayOfWeek), 36, dayOfWeek.c_str());
  } while (u8g2.nextPage());
}

void displayClock()
{
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


void oldDisplayAlarm()
{
  u8g2.firstPage();
  String alarmText = "ALARM";

  digitalWrite(led, HIGH);
  tone(buzzer, 350);
  do
  {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(ALIGN_CENTER(alarmText), 36, alarmText.c_str());
  } while (u8g2.nextPage());
  delay(500);
  digitalWrite(led, LOW);
  noTone(buzzer);
  u8g2.firstPage();
  do
  {
    // display blank screen
  } while (u8g2.nextPage());
  delay(500);
}

void displayAlarm()
{
  String alarmText = "ALARM";

  for (int x = 0; x < tuneLength; x++)
  {
    tone(buzzer, tune[x]);
    digitalWrite(led, HIGH);
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(alarmText), 36, alarmText.c_str());
    } while (u8g2.nextPage());
    delay(250 * durt[x]);
    digitalWrite(led, LOW);
    u8g2.firstPage();
    do
    {
      // display blank screen
    } while (u8g2.nextPage());
    delay(50 * durt[x]);
    noTone(buzzer);
  }
}

void loop()
{
  clock.getTime();
  if (clock.hour == alarmHour && clock.minute == alarmMinute)
  {
    displayAlarm();
  }
  else
  {
    displayClock();
    //displayAlarm();
  }
  delay(500);
}
