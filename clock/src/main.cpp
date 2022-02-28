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

// default alarm time
int alarmHour = 8;
int alarmMinute = 0;
bool alarmOn = false;

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
#define ALARM_TOGGLE_MODE 5

char alarmCanceled = false;

void serialDebug(String message)
{
  // Serial.println(message);
}

void Checker()
{
  static char LEDBlink = 0;
  if (MODE == ALARM_PLAYBACK_MODE)
  {
    if (digitalRead(button) == HIGH)
    {
      serialDebug("Cancelling Alarm");
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

String getDayOfWeek(int dayOfWeek)
{
  String theDayOfWeek;
  switch (dayOfWeek)
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

String getMonth(int month)
{
  String theMonth;
  switch (month)
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

bool isLeapYear(int year)
{
  // If a year is multiple of 400,
  // then it is a leap year
  if (year % 400 == 0)
    return true;

  // Else If a year is multiple of 100,
  // then it is not a leap year
  if (year % 100 == 0)
    return false;

  // Else If a year is multiple of 4,
  // then it is a leap year
  if (year % 4 == 0)
    return true;
  return false;
}

int daysInMonth(int month, int year)
{
  int days = 0;
  switch (month)
  {
  case 1:
    days = 31;
    break;
  case 2:
    days = isLeapYear(year) ? 29 : 28;
    break;
  case 3:
    days = 31;
    break;
  case 4:
    days = 30;
    break;
  case 5:
    days = 31;
    break;
  case 6:
    days = 30;
    break;
  case 7:
    days = 31;
    break;
  case 8:
    days = 31;
    break;
  case 9:
    days = 30;
    break;
  case 10:
    days = 31;
    break;
  case 11:
    days = 30;
    break;
  case 12:
    days = 31;
    break;
  }
  return days;
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
    displayDayOfWeek(getDayOfWeek(clock.dayOfWeek));
  }
  else
  {
    displayDate(String(clock.dayOfMonth, DEC), getMonth(clock.month));
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

void selectMode(int n)
{
  String debug = "reached select_mode, mode is[";
  debug += String(n, DEC);
  debug += "]";
  serialDebug(debug);
  switch (n)
  {
  case CLOCK_SET_MODE:
    u8g2.firstPage();
    do
    {
      String displayText = "Time Set";
      u8g2.setFont(u8g2_font_t0_16b_mr);
      u8g2.drawXBMP(49, 16, 30, 30, light_tmp);
      u8g2.setCursor(ALIGN_CENTER(displayText), 46);
      u8g2.print(F("Time Set")); // write something to the internal memory
    } while (u8g2.nextPage());
    break;
  case ALARM_SET_MODE:
    u8g2.firstPage();
    do
    {
      String displayText = "Alarm Set";
      u8g2.setFont(u8g2_font_t0_16b_mr);
      u8g2.drawXBMP(49, 8, 30, 30, sound_bmp1);
      u8g2.setCursor(ALIGN_CENTER(displayText), 48);
      u8g2.print(F("Alarm Set")); // write something to the internal memory
    } while (u8g2.nextPage());
    break;
  case ALARM_TOGGLE_MODE:
    u8g2.firstPage();
    do
    {
      String displayText = "Alarm On/Off";
      u8g2.setFont(u8g2_font_t0_16b_mr);
      u8g2.drawXBMP(49, 8, 30, 30, sound_bmp1);
      u8g2.setCursor(ALIGN_CENTER(displayText), 48);
      u8g2.print(F("Alarm On/Off")); // write something to the internal memory
    } while (u8g2.nextPage());
    break;
  }
}

void setHourAndMinute(int *hour, int *minute)
{
  int newHour = *hour;
  int initialRotaryPosition = analogRead(rotary) / 204.8;
  int currentRotaryPosition = initialRotaryPosition;
  while (1)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(String(newHour, DEC)), 36, String(newHour, DEC).c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      newHour++;
      newHour = newHour % 24;
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  delay(1000);
  int newMinute = *minute;
  while (1)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(String(newMinute, DEC)), 36, String(newMinute, DEC).c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      newMinute++;
      newMinute = newMinute % 60;
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  *hour = newHour;
  *minute = newMinute;
}
// stuff for the main loop

int setYear()
{
  int newYear = 2020;
  int initialRotaryPosition = analogRead(rotary) / 204.8;
  int currentRotaryPosition = initialRotaryPosition;
  while (1)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(String(newYear, DEC)), 36, String(newYear, DEC).c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      newYear++;
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  delay(1000);
  return newYear - 2000;
}

int setMonth(int oldMonth)
{
  int newMonth = oldMonth;

  int initialRotaryPosition = analogRead(rotary) / 204.8;
  int currentRotaryPosition = initialRotaryPosition;
  while (1)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(getMonth(newMonth)), 36, getMonth(newMonth).c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      newMonth++;
      if (newMonth > 12)
      {
        newMonth = newMonth - 12;
      }
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  delay(1000);
  return newMonth;
}

int setDayOfMonth(int oldDayOfMonth, int numDaysInMonth)
{
  int newDayOfMonth = oldDayOfMonth;

  int initialRotaryPosition = analogRead(rotary) / 204.8;
  int currentRotaryPosition = initialRotaryPosition;
  while (1)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(String(newDayOfMonth, DEC)), 36, String(newDayOfMonth, DEC).c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      newDayOfMonth++;
      if (newDayOfMonth > numDaysInMonth)
      {
        newDayOfMonth = newDayOfMonth - numDaysInMonth;
      }
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  delay(1000);
  return newDayOfMonth;
}

int setDayOfWeek(int oldDayOfWeek)
{
  int newDayOfWeek = oldDayOfWeek;

  int initialRotaryPosition = analogRead(rotary) / 204.8;
  int currentRotaryPosition = initialRotaryPosition;
  while (1)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(getDayOfWeek(newDayOfWeek)), 36, getDayOfWeek(newDayOfWeek).c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      newDayOfWeek++;
      if (newDayOfWeek > 7)
      {
        newDayOfWeek = newDayOfWeek - 7;
      }
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  delay(1000);
  return newDayOfWeek;
}

void setAlarm()
{
  setHourAndMinute(&alarmHour, &alarmMinute);
  MODE = CLOCK_MODE;
}

void setClock()
{
  int newHour = clock.hour;
  int newMinute = clock.minute;
  int newYear = clock.year;
  int newMonth = clock.month;
  int newDayOfMonth = clock.dayOfMonth;
  int newDayOfWeek = clock.dayOfWeek;

  newYear = setYear();
  newMonth = setMonth(newMonth);
  newDayOfMonth = setDayOfMonth(newDayOfMonth, daysInMonth(newMonth, newYear));
  newDayOfWeek = setDayOfWeek(newDayOfWeek);
  setHourAndMinute(&newHour, &newMinute);

  clock.fillByYMD(newYear, newMonth, newDayOfMonth);
  clock.fillByHMS(newHour, newMinute, 30); // 30 seconds
  clock.fillDayOfWeek(newDayOfWeek);
  clock.setTime(); // write time to the RTC chip
  MODE = CLOCK_MODE;
}

void toggleAlarm()
{
  int initialRotaryPosition = analogRead(rotary) / 204.8;
  int currentRotaryPosition = initialRotaryPosition;
  while (1)
  {
    String displayString;
    if (alarmOn)
    {
      displayString = "Alarm On";
    }
    else
    {
      displayString = "Alarm Off";
    }
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(ALIGN_CENTER(displayString), 36, displayString.c_str());
    } while (u8g2.nextPage());
    currentRotaryPosition = analogRead(rotary) / 204.8;
    if (currentRotaryPosition != initialRotaryPosition)
    {
      initialRotaryPosition = currentRotaryPosition;
      alarmOn = !alarmOn;
    }
    if (digitalRead(button) == HIGH)
    {
      delay(4);
      if (digitalRead(button) == HIGH)
      {
        break;
      }
    }
  }
  delay(1000);
  MODE = CLOCK_MODE;
}

char LongPress = false;

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
  // Serial.begin(9600);
}

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
            int n = analogRead(rotary) / 341.33 + 3;
            digitalWrite(led, HIGH);
            digitalWrite(buzzer, LOW);
            selectMode(n);
            if (digitalRead(button) == HIGH)
            {
              delay(4);
              if (digitalRead(button) == HIGH)
              {
                MODE = analogRead(rotary) / 341.33 + 3;
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

  String debug = "in loop, mode is [" + String(MODE, DEC) + "]";
  serialDebug(debug);
  clock.getTime();
  if (MODE == CLOCK_MODE)
  {
    if (alarmOn && clock.hour == alarmHour && clock.minute == alarmMinute)
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
      oldDisplayAlarm();
      delay(100);
    }
  }
  else if (MODE == CLOCK_SET_MODE)
  {
    setClock();
  }
  else if (MODE == ALARM_SET_MODE)
  {
    setAlarm();
  }
  else if (MODE == ALARM_TOGGLE_MODE)
  {
    toggleAlarm();
  }
}