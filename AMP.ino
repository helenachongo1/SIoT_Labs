#include "BluetoothA2DPSink.h"

#define BTN_POWER 23
#define BTN_VOLUP 22
#define BTN_VOLDN 21

#define LED_POWER 19
#define LED_VOLUP 18
#define LED_VOLDN 5

BluetoothA2DPSink a2dp_sink;

bool powerState = false;
bool muted = false;
int volume = 20;   // safer default (0–127)

// timing 
const unsigned long DOUBLE_PRESS_TIME = 250;
const unsigned long LONG_PRESS_TIME   = 600;

// state 
unsigned long lastUpPress = 0;
unsigned long lastDownPress = 0;
bool waitDoubleUp = false;
bool waitDoubleDown = false;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_POWER, INPUT_PULLUP);
  pinMode(BTN_VOLUP, INPUT_PULLUP);
  pinMode(BTN_VOLDN, INPUT_PULLUP);

  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_VOLUP, OUTPUT);
  pinMode(LED_VOLDN, OUTPUT);

  Serial.println("Ready — press POWER");
}

void startBT() {
  Serial.println("Bluetooth ON");
  digitalWrite(LED_POWER, HIGH);

  i2s_pin_config_t pins = {
    .bck_io_num = 26,
    .ws_io_num = 27,
    .data_out_num = 25,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  a2dp_sink.set_pin_config(pins);
  a2dp_sink.start("ESP32-Headphones");
  a2dp_sink.set_volume(volume);
}

void stopBT() {
  Serial.println("Bluetooth OFF");
  digitalWrite(LED_POWER, LOW);
  a2dp_sink.end();
}

// MAIN LOOP
void loop() {

  // POWER BUTTON
  if (digitalRead(BTN_POWER) == LOW) {
    delay(200);
    powerState = !powerState;

    if (powerState) startBT();
    else stopBT();

    while (digitalRead(BTN_POWER) == LOW);
  }

  // VOL UP BUTTON
  if (digitalRead(BTN_VOLUP) == LOW) {
    unsigned long pressStart = millis();

    while (digitalRead(BTN_VOLUP) == LOW) {
      if (millis() - pressStart > LONG_PRESS_TIME) {
        Serial.println("NEXT TRACK");
        a2dp_sink.next();
        digitalWrite(LED_VOLUP, HIGH);
        delay(200);
        digitalWrite(LED_VOLUP, LOW);
        while (digitalRead(BTN_VOLUP) == LOW);
        return;
      }
    }

    if (waitDoubleUp && millis() - lastUpPress < DOUBLE_PRESS_TIME) {
      waitDoubleUp = false;
      muted = !muted;
      a2dp_sink.set_volume(muted ? 0 : volume);
      Serial.println(muted ? "MUTED" : "UNMUTED");
    } else {
      waitDoubleUp = true;
      lastUpPress = millis();
    }
  }

  if (waitDoubleUp && millis() - lastUpPress > DOUBLE_PRESS_TIME) {
    waitDoubleUp = false;
    if (volume < 127) volume += 5;
    if (!muted) a2dp_sink.set_volume(volume);
    Serial.print("Volume: ");
    Serial.println(volume);
  }

  // VOL DOWN BUTTON 
  if (digitalRead(BTN_VOLDN) == LOW) {
    unsigned long pressStart = millis();

    while (digitalRead(BTN_VOLDN) == LOW) {
      if (millis() - pressStart > LONG_PRESS_TIME) {
        Serial.println("PREVIOUS TRACK");
        a2dp_sink.previous();
        digitalWrite(LED_VOLDN, HIGH);
        delay(200);
        digitalWrite(LED_VOLDN, LOW);
        while (digitalRead(BTN_VOLDN) == LOW);
        return;
      }
    }

    if (waitDoubleDown && millis() - lastDownPress < DOUBLE_PRESS_TIME) {
      waitDoubleDown = false;
      muted = !muted;
      a2dp_sink.set_volume(muted ? 0 : volume);
      Serial.println(muted ? "MUTED" : "UNMUTED");
    } else {
      waitDoubleDown = true;
      lastDownPress = millis();
    }
  }

  if (waitDoubleDown && millis() - lastDownPress > DOUBLE_PRESS_TIME) {
    waitDoubleDown = false;
    if (volume > 0) volume -= 5;
    if (!muted) a2dp_sink.set_volume(volume);
    Serial.print("Volume: ");
    Serial.println(volume);
  }
}

