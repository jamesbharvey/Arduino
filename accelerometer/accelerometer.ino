#include "LIS3DHTR.h"
#ifdef SOFTWAREWIRE
#include <SoftwareWire.h>
SoftwareWire myWire(3, 2);
LIS3DHTR<SoftwareWire> LIS;
#define WIRE myWire
#else
#include <Wire.h>
LIS3DHTR<TwoWire> LIS;
#define WIRE Wire
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {};
  LIS.begin(WIRE, 0x19);
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!LIS) {
    Serial.println("LIS3DHTR didn't connect.");
    while (1);
    return;
  }
  Serial.print("x:"); Serial.print(LIS.getAccelerationX()); Serial.print("  ");
  Serial.print("y:"); Serial.print(LIS.getAccelerationY()); Serial.print("  ");
  Serial.print("z:"); Serial.println(LIS.getAccelerationZ());

  delay(500);

}
