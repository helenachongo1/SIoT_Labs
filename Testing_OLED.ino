/*
  Filename: esp32_oled_typewriter.ino
  Description: Displays "OceanLabz" on OLED with typewriter animation using ESP32
  Author: www.oceanlabz.in
  Modification: 1/4/2025
 
 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
// Define screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
 
// Define I2C pins for ESP32 (modify if needed)
#define OLED_SDA 21
#define OLED_SCL 22
 
// Create display object with custom I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
void setup() {
  Serial.begin(115200);
 
  // Start I2C on custom pins (for ESP32)
  Wire.begin(OLED_SDA, OLED_SCL);
 
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    while (true);  // Stop execution if display fails to initialize
  }
 
  display.clearDisplay();
  display.display();
 
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}
 
void loop() {
  display.clearDisplay();
  display.setCursor(0, 20);
 
  const char* text = "OceanLabz";
 
  // Typewriter animation
  for (int i = 0; i < strlen(text); i++) {
    display.print(text[i]);
    display.display();
    delay(200);
  }
 
  delay(2000);
 
  display.clearDisplay();
  display.display();
  delay(500);
}*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 15
#define OLED_SCL 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

  Serial.begin(115200);
  delay(2000);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  Serial.println("OLED OK");
}

void loop() {

  display.clearDisplay();
  display.setCursor(0,20);

  const char* text = "ESP32-CAM";

  for (int i = 0; i < strlen(text); i++) {
    display.print(text[i]);
    display.display();
    delay(200);
  }

  delay(2000);

  display.clearDisplay();
  display.display();
  delay(500);
}