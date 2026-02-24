#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>

// there is no 72x40 constructor in u8g2 hence the 72x40 screen is
// mapped in the middle of the 132x64 pixel buffer of the SSD1306 controller
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);
int width = 68;
int height = 62;
int xOffset = (132-width)/2;
int yOffset = 24; //(64-height)/2;

void setup(void)
{
     delay(1000);
     u8g2.begin();
     u8g2.setContrast(255); // set contrast to maximum 
     u8g2.setBusClock(400000); //400kHz I2C 
     u8g2.setFont(u8g2_font_ncenB24_tr);
 //    u8g2.setFont(u8g2_font_helvR24_tf);
}    

void loop(void)
{
    u8g2.clearBuffer(); // clear the internal memory
    //u8g2.drawFrame(xOffset+0, yOffset+0, width, height); //draw a frame around the border
    u8g2.setCursor(xOffset+4, yOffset+30);
    u8g2.printf("%dC", width);
    u8g2.sendBuffer(); 
}