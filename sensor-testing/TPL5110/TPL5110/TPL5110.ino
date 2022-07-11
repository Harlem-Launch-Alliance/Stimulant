void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  delay(3000);               // Delay for 3 seconds
  digitalWrite(2, HIGH);    // Sends signal for power to cut off
}
