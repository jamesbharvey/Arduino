#include "Arduino.h"
#include <Wire.h>
#include <DS1307.h>   // Realtime clock library
#include <U8g2lib.h>  //OLED Display library
#include <MsTimer2.h> //Simplified Interupt Timer Library
#include "music.h"
#include "bmps.h"

DS1307 clock; // define a object of DS1307 class
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE);

// pins
char led = 4;
char buzzer = 5;
char button = 6;
char rotary = A0;

// hardcoded default alarm time
int alarmHour = 22;
int alarmMinute = 01;

#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t.c_str()))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t.c_str()))
#define ALIGN_LEFT 0

// stuff for our Interrupt checker
char PressCounter = 0;
char BlinkEnable = true;
char MODE = 1;

#define BLINK 10
#define CLICKS 100
#define CLOCK_MODE 1
#define ALARM_PLAYBACK_MODE 2
#define ALARM_SET_MODE 3
#define CLOCK_SET_MODE 4
char alarmCanceled = false;

void Checker()
{
  static char LEDBlink = 0;
  if (MODE == ALARM_PLAYBACK_MODE)
  {
    if (digitalRead(button) == HIGH)
    {
      Serial.println("Cancelling Alarm");
      MODE = CLOCK_MODE;
      alarmCanceled = true;
      return;
    }
  }
  if (BlinkEnable)
  {
    digitalWrite(led, HIGH);
    LEDBlink++;
    if (LEDBlink > BLINK)
    {
      LEDBlink = 0;
      BlinkEnable = false;
      digitalWrite(led, LOW);
    }
  }
  if (digitalRead(button) == HIGH)
  {
    PressCounter++;
    if (PressCounter > CLICKS)
    {
      PressCounter = CLICKS + 1;
    }
  }
  else
  {
    PressCounter = 0;
  }
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
  MsTimer2::set(10, Checker); // 10ms period
  MsTimer2::start();
  Serial.begin(9600);
}

/* void loop()
{
  clock.getTime();
  if (clock.hour == alarmHour && clock.minute == alarmMinute)
  {
    displayAlarm();
  }
  else
  {
    //displayClock();
    displayAlarm();
  }
  delay(500);
}
 */
void select_mode(int n)
{
  Serial.print("reached select_mode, mode is[");
  String debug = String(n, DEC);
  debug += "]";
  Serial.println(debug);
  switch (n)
  {
  case 1:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_t0_16b_mr);
      u8g2.drawXBMP(49, 16, 30, 30, light_tmp);
      u8g2.setCursor(44, 46);
      u8g2.print(F("Light")); // write something to the internal memory
    } while (u8g2.nextPage());
    break;
  case 2:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_t0_16b_mr);
      u8g2.drawXBMP(49, 8, 30, 30, sound_bmp1);
      u8g2.setCursor(44, 48);
      u8g2.print(F("Sound")); // write something to the internal memory
    } while (u8g2.nextPage());
    break;
  }
}

// stuff for the main loop

char LongPress = false;

void loop()
{
  if (digitalRead(button) == HIGH)
  {
    delay(5);
    if (digitalRead(button) == HIGH)
    {
      BlinkEnable = true;
      if (PressCounter > CLICKS)
      {
        PressCounter = 0;
        if (digitalRead(button) == HIGH)
        {
          LongPress = true;
          while (digitalRead(button))
          {
            if (LongPress)
            {
              LongPress = false;
              tone(buzzer, 300);
              delay(200);
              noTone(buzzer);
            }
          }
          while (1)
          {
            int n = analogRead(rotary) / 204.8 + 1;
            digitalWrite(led, HIGH);
            digitalWrite(buzzer, LOW);
            select_mode(n);
            if (digitalRead(button) == HIGH)
            {
              delay(4);
              if (digitalRead(button) == HIGH)
              {
                MODE = analogRead(rotary) / 204.8 + 1;
                while (digitalRead(button))
                  ;
                digitalWrite(led, LOW);
                digitalWrite(buzzer, LOW);
                delay(500);
                break;
              }
            }
          }
        }
      }
    }
  }
  Serial.print("in loop, mode is [");
  String debug = String(MODE, DEC);
  Serial.println(debug);
  clock.getTime();
  if (MODE == CLOCK_MODE)
  {
    if (clock.hour == alarmHour && clock.minute == alarmMinute)
    {
      MODE = ALARM_PLAYBACK_MODE;
    }
    else
    {
      if (alarmCanceled)
      {
        alarmCanceled = false;
      }
    }
    displayClock();
    delay(500);
  }
  else if (MODE == ALARM_PLAYBACK_MODE)
  {
    if (!alarmCanceled)
    {
      displayAlarm();
    }
  }
}