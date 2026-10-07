const int piezoPin = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int vibration = analogRead(piezoPin);
  Serial.println(vibration);
  delay(10);
}