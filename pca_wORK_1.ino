#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); 
  Serial.println("Bluetooth Started");
}

void loop() {
  
  SerialBT.println("Hello from ESP32!");
  
  Serial.println("Message sent to mobile");
  
  delay(2000);
}
