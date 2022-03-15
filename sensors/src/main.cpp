#include "Seeed_HM330X.h"
  
HM330X sensor;
u8 buf[30];

typedef HM330XErrorCode err_t;

const char *old_str[]={"sensor num: ","PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };
 
const char *str[]={"sensor num:",
                    "pm1spm",
                    "pm2p5spm",
                    "pm10spm",
                    "pm1ae",
                    "pm2p5ae",
                    "pm10ae",
                    };
 
/*parse buf with 29 u8-data*/
err_t parse_result(u8 *data)
{
    u16 value=0;
    if(NULL==data)
        return ERROR_PARAM;
    //print in the format expected by graphite's line protocol
    for(int i=2;i<8;i++)
    {
        value = (u16)data[i*2]<<8|data[i*2+1];
        Serial.print("mysensors.bedroom.");
        Serial.print(str[i-1]);
        Serial.print(" ");
        Serial.print(value);
        Serial.println(" -1"); // -1 tells graphite to use time received
        delay(1000);
    }
    return NO_ERROR;
}
 
err_t parse_result_value(u8 *data)
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
 
 
/*30s*/
void setup()
{
    Serial.begin(9600);
    delay(100);
    //Serial.println("Serial start");
    if(sensor.init())
    {
        Serial.println("HM330X init failed!!!");
        while(1);
    }
 
}
 
 
 
void loop()
{
    if(sensor.read_sensor_value(buf,29))
    {
        Serial.println("HM330X read result failed!!!");
    }
    parse_result_value(buf);
    parse_result(buf);
    delay(60000);
}