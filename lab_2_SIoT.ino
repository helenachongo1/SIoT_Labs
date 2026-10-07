#define BLYNK_TEMPLATE_ID "TMPL3xqbDLZtq"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "HmdedXxovYZz3WzG_WHAvsyicSwP07Ds"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Galaxy A25 5G E50E";
char pass[] = "11111111";

#define LDR_PIN 34   // The analog pin

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();

  int ldrValue = analogRead(LDR_PIN);
  Blynk.virtualWrite(V5, ldrValue); 
  Serial.println(ldrValue);
  delay(1000);
}