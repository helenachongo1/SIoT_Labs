#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3xqbDLZtq"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "FDDCwUXH2fbfzjlcMVhvYjDTGet8fcT_"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Galaxy A25 5G E50E";
char pass[] = "11111111";

int ledPin = 13;

BLYNK_WRITE(V7)
{
  int value = param.asInt();
  digitalWrite(ledPin, value);
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
}