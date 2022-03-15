/*
  Hello World.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include "rgb_lcd.h"
#define RXD2 16
#define TXD2 17

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

char ap_buf[30][16];
int ap_cnt = 0;

void setup() 
{
    
    Serial.begin(115200);
 
    delay(3000);
    Wire.begin();
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
    lcd.print("hello, world!");

    delay(1000);
}
// send command
void cmd_send(const char *cmd)
{
    if(NULL == cmd)return;
    Serial.println(cmd);
}
 
 
// wait result of ap scan
// +CWLAP:(3,"360WiFi-UZ",-81,"08:57:00:01:61:ec",1)
void wait_result()
{
    while(1)
    {
LOOP1:
        char c1=0;
        if(Serial.available()>=2)
        {
            c1 = Serial.read();
            if(c1 == 'O' && 'K' == Serial.read())return;       // OK means over
        }
 
        if('('==c1)
        {
            while(Serial.available()<3);
            Serial.read();
            Serial.read();
            Serial.read();
 
            int d = 0;
            while(1)
            {
                if(Serial.available() && '"' == Serial.read());      // find "
                {
                    while(1)
                    {
                        if(Serial.available())
                        {
                            char c = Serial.read();
                            ap_buf[ap_cnt][d++] = c;
                            if(c == '"' || d==16)
                            {
                                ap_buf[ap_cnt][d-1] = '\0';
                                ap_cnt++;
                                goto LOOP1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void loop() 
{
    String the_cmd = "AT+CWLAP";
    cmd_send(the_cmd.c_str());
    wait_result();
 
    display_ap();
    delay(5000);
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    //lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    //lcd.print(millis()/1000);

    delay(100);
}
void display_ap()
{
    char strtmp[16];
    sprintf(strtmp, "get %d ap", ap_cnt);
 
    delay(2000);
 
    int cnt = ap_cnt;
    int offset = 0;
    while(1)
    {
        if(cnt>=8)
        {
            for(int i=0; i<8; i++)
            {
                lcd.print(ap_buf[8*offset+i]);        // Print the String
            }
            cnt-=8;
            offset++;
        }
        else 
        {
            for(int i=0; i<cnt; i++)
            {
                lcd.print(ap_buf[8*offset+i]);        // Print the String
            }
 
            return;
        }
 
        delay(2000);
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
