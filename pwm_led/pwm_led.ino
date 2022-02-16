const int ledPin = 3;
const int potentiometerPin = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(potentiometerPin,INPUT);
}

void loop() {
  int potentioValue, Value;
  potentioValue = analogRead(potentiometerPin);
  Value = map(potentioValue,0,1023,0,255);
  analogWrite(ledPin,Value);
  // put your main code here, to run repeatedly:
}
