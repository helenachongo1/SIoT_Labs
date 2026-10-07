#define BLYNK_TEMPLATE_ID           "TMPL3xqbDLZtq"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "pDGKz525WetM7B48qHTJ48rD35pCciIV"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Galaxy A25 5G E50E";
char pass[] = "11111111";

#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.println(distance);

  Blynk.virtualWrite(V4, distance);

  delay(1000);
}