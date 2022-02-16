const int lightSensorPin = A6;
const int ledPin = 4;
int sensorValue = 0;
int outputValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(lightSensorPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(lightSensorPin);
  Serial.println(sensorValue);
  if (sensorValue < 200) {
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }
  delay(200);
}
