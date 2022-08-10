#include <Arduino.h>
#include "Seeed_HM330X.h"
#include "Seeed_BMP280.h" // air pressure sensor
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

#define DHTPIN 2
#define DHTTYPE DHT11
#define GRAPHITETREEHEAD "mysensors.bedroom."

DHT dht(DHTPIN, DHTTYPE);
HM330X dustSensor;
u8 dustSensorBuf[30];
BMP280 bmp280;
const int lightSensorPin = A0;
const int soundSensorPin = A3;
float PM25GLOBAL;
String FORSCROLLINGGLOBAL[4];


typedef HM330XErrorCode DustSensorError;

const char *fullDustSensorMetricNameStr[] = {
    "sensor num: ",
    "PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
    "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
    "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
    "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
    "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
    "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
};

const char *dustSensorMetricNameStr[] = {
    "sensor num:",
    "pm1spm",
    "pm2p5spm",
    "pm10spm",
    "pm1ae",
    "pm2p5ae",
    "pm10ae",
};

/*parse dustSensorBuf with 29 u8-data*/
DustSensorError dustSensorParsePrintResult(u8 *data)
{
  u16 value = 0;
  if (NULL == data)
    return ERROR_PARAM;
  // print in the format expected by graphite's line protocol
  for (int i = 2; i < 8; i++)
  {
    value = (u16)data[i * 2] << 8 | data[i * 2 + 1];
    Serial.print(GRAPHITETREEHEAD);
    Serial.print(dustSensorMetricNameStr[i - 1]);
    Serial.print(" ");
    Serial.print(value);
    Serial.println(" -1"); // -1 tells graphite to use time received
    if (i == 5)
    {
      display.print(F("PM 2.5m: "));
      display.print(value);
      display.println(F(" ug/m3"));
      display.display();
      PM25GLOBAL = value; // ick....
      String thePM25 = "PM2.5m: " + String(value) + "\nug/m3";
      FORSCROLLINGGLOBAL[3] = thePM25;
    }
    delay(1000);
  }
  return NO_ERROR;
}

DustSensorError dustSensorParseResultValue(u8 *data)
{
  if (NULL == data)
    return ERROR_PARAM;
  // for(int i=0;i<28;i++)
  // {
  //     Serial.print(data[i],HEX);
  //     Serial.print("  ");
  //     if((0==(i)%5)||(0==i))
  //     {
  //         Serial.println(" ");
  //     }
  // }
  u8 sum = 0;
  for (int i = 0; i < 28; i++)
  {
    sum += data[i];
  }
  if (sum != data[28])
  {
    Serial.println("HM330X wrong checkSum!!!!");
  }
  return NO_ERROR;
}

void setup()
{
  Serial.begin(9600);
  delay(100);
  // Serial.println("Serial start");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  if (dustSensor.init())
  {
    Serial.println("HM330X init failed!!!");
    while (1)
      ;
  }
  dht.begin();
  if (!bmp280.init())
  {
    Serial.println("BMP280 not connected or broken!");
  }
  randomSeed(42);
}

void swipetoptobottom(void)
{
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  // vertical swipe
  for (i = 0; i < display.height(); i += 1)
  {
    display.drawLine(0, i, display.width(), i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for (i = 0; i < display.height(); i += 1)
  {
    display.drawLine(0, i, display.width(), i, SSD1306_BLACK);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
}

void swipebottomtotop(void)
{
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  // vertical swipe
  for (i = display.height() - 1; i > 0; i -= 1)
  {
    display.drawLine(0, i, display.width(), i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for (i = display.height() - 1; i > 0; i -= 1)
  {
    display.drawLine(0, i, display.width(), i, SSD1306_BLACK);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
}

void loop()
{

  display.clearDisplay();
  display.display();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner


  float temp;
  temp = dht.readTemperature();
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("temperature ");
  Serial.print(temp);
  Serial.println(" -1"); // -1 tells graphite to use time received
  display.print(F("Temperature: "));
  display.print(temp);
  display.print(char(9));
  display.println(F("C"));
  display.display();
  String theTemp = "Temp:" + String(temp, 0) + char(9) + "C";
  FORSCROLLINGGLOBAL[0] = theTemp;
  delay(1000);

  float humi;
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("humidity ");
  humi = dht.readHumidity();
  Serial.print(humi);
  Serial.println(" -1"); // -1 tells graphite to use time received
  display.print(F("Humidity: "));
  display.print(humi);
  display.println(F("%"));
  display.display();
  String theHumi = "Humi: " + String(humi, 0) + "%";
  FORSCROLLINGGLOBAL[1] = theHumi;
  delay(1000);

  float Bmp280Temp;
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("alt.temperature ");
  Bmp280Temp = bmp280.getTemperature();
  Serial.print(Bmp280Temp);
  Serial.println(" -1"); // -1 tells graphite to use time received
  delay(1000);

  float pressure;
  pressure = bmp280.getPressure();
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("pressure ");
  Serial.print(pressure);
  Serial.println(" -1"); // -1 tells graphite to use time received
  display.print(F("BP: "));
  display.print(pressure);
  display.println(F(" hPa"));
  display.display();
  String theBP = "BP:" + String(pressure,0) + "\nhPa";
  FORSCROLLINGGLOBAL[2] = theBP;
  delay(1000);

  float altitude;
  altitude = bmp280.calcAltitude(pressure);
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("altitude ");
  Serial.print(altitude);
  Serial.println(" -1"); // -1 tells graphite to use time received
  delay(1000);

  int lightValue = 0;
  lightValue = analogRead(lightSensorPin);
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("light ");
  Serial.print(lightValue);
  Serial.println(" -1"); // -1 tells graphite to use time received
  delay(1000);

  int soundValue = 0;
  soundValue = analogRead(soundSensorPin);
  Serial.print(GRAPHITETREEHEAD);
  Serial.print("sound ");
  Serial.print(soundValue);
  Serial.println(" -1"); // -1 tells graphite to use time received
  delay(1000);

  if (dustSensor.read_sensor_value(dustSensorBuf, 29))
  {
    Serial.println("HM330X read result failed!!!");
  }
  dustSensorParseResultValue(dustSensorBuf);
  dustSensorParsePrintResult(dustSensorBuf);
  delay(2000);

  display.clearDisplay();

  swipetoptobottom();
  delay(1000);
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.setTextWrap(true);
  int toScrollIndex = random(3);
  display.setCursor(0,0);
  if (toScrollIndex < 2) {
    display.setCursor(10,11);
  } 
  display.println(FORSCROLLINGGLOBAL[toScrollIndex]);
  display.display(); // Show initial text
  delay(100);
  display.startscrollleft(0x00, 0x07);
  delay(15000); /*15s*/
  display.stopscroll();
  display.clearDisplay();
  swipebottomtotop();
  display.clearDisplay();
}