const int soundSensorPin = A2;
const int ledPin = 4;
int sensorValue = 0;
int outputValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(soundSensorPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(soundSensorPin);
  Serial.println(sensorValue);
  if (sensorValue > 275) {
    digitalWrite(ledPin,HIGH);
    delay(100);
  } else {
    digitalWrite(ledPin,LOW);
  }
  delay(100);
}
