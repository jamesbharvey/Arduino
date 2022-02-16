void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// initializes the serial port with a baud rate of 9600
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("hello, world");
  delay(1000);
}
