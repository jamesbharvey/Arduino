#include <Arduino.h>
#include "Seeed_HM330X.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define GRAPHITETREEHEAD "mysensors.bedroom."

DHT dht(DHTPIN, DHTTYPE);
HM330X dustSensor;
u8 dustSensorBuf[30];

typedef HM330XErrorCode DustSensorError;

const char *fullDustSensorMetricNameStr[]={
                    "sensor num: ",
                    "PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };
 
const char *dustSensorMetricNameStr[]={
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
    u16 value=0;
    if(NULL==data)
        return ERROR_PARAM;
    //print in the format expected by graphite's line protocol
    for(int i=2;i<8;i++)
    {
        value = (u16)data[i*2]<<8|data[i*2+1];
        Serial.print(GRAPHITETREEHEAD);
        Serial.print(dustSensorMetricNameStr[i-1]);
        Serial.print(" ");
        Serial.print(value);
        Serial.println(" -1"); // -1 tells graphite to use time received
        delay(1000);
    }
    return NO_ERROR;
}
 
DustSensorError dustSensorParseResultValue(u8 *data)
{
    if(NULL==data)
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
    u8 sum=0;
    for(int i=0;i<28;i++)
    {
        sum+=data[i];
    }
    if(sum!=data[28])
    {
        Serial.println("HM330X wrong checkSum!!!!");
    }
    return NO_ERROR;
}
 
 

void setup()
{
    Serial.begin(9600);
    delay(100);
    //Serial.println("Serial start");
    if(dustSensor.init())
    {
        Serial.println("HM330X init failed!!!");
        while(1);
    }
    dht.begin();
 
}
 
void loop()
{
    if(dustSensor.read_sensor_value(dustSensorBuf,29))
    {
        Serial.println("HM330X read result failed!!!");
    }
    dustSensorParseResultValue(dustSensorBuf);
    dustSensorParsePrintResult(dustSensorBuf);

    float temp;
    temp = dht.readTemperature();
    Serial.print(GRAPHITETREEHEAD);
    Serial.print("temperature ");
    Serial.print(temp);
    Serial.println(" -1"); // -1 tells graphite to use time received
    delay(1000);

    float humi;
    Serial.print(GRAPHITETREEHEAD);
    Serial.print("humidity ");
    humi = dht.readHumidity();
    Serial.print(humi);
    Serial.println(" -1"); // -1 tells graphite to use time received
    delay(1000);

    delay(22000); /*22s*/
}