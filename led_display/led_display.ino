#include <Arduino.h>
#include <U8x8lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  // put your setup code here, to run once:
  u8x8.begin();
  u8x8.setFlipMode(1);
}

void loop(void) {
  // put your main code here, to run repeatedly:
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(0, 0);
    u8x8.print("Hello World!");
}
