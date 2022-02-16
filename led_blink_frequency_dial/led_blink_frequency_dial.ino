const int rotaryPin = A0;
const int ledPin = 4;
int rotaryValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(rotaryPin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  rotaryValue = analogRead(rotaryPin);
  delay(rotaryValue);
  digitalWrite(ledPin,HIGH);
  delay(rotaryValue);
  digitalWrite(ledPin,LOW);
}
