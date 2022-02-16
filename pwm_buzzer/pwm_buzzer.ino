const int buzzerPin = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzerPin,OUTPUT);
}

void loop() {
  analogWrite(buzzerPin,128);
  delay(1000);
  analogWrite(buzzerPin,0);
  delay(0);
  // put your main code here, to run repeatedly:
}
