#define BLYNK_TEMPLATE_ID "TMPL3xqbDLZtq"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "n0XSFdvpPwEdEN76bNK318lGKTzqltAy"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Galaxy A25 5G E50E";
char pass[] = "11111111";

int relayPin = 26;

void setup()
{
  Serial.begin(115200);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Connected to Blynk");
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  if(value == 1)
  {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay ON");
  }
  else
  {
    digitalWrite(relayPin, HIGH);
    Serial.println("Relay OFF");
  }
}

void loop()
{
  Blynk.run();
}