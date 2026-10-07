#define LED1_PIN 2
#define LED2_PIN 4
#define BUTTON_PIN 23

TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;

// ISR variables
volatile bool task1Suspended = false;
volatile uint32_t lastInterruptTime = 0;
const uint32_t debounceDelay = 100;

// BUTTON INTERRUPT
void IRAM_ATTR buttonISR() {

  uint32_t currentTime = millis();

  if (currentTime - lastInterruptTime < debounceDelay) {
    return;
  }

  lastInterruptTime = currentTime;

  task1Suspended = !task1Suspended;

  if (task1Suspended) {
    vTaskSuspend(Task1Handle);
  } else {
    vTaskResume(Task1Handle);
  }
}

// TASK 1
void Task1(void *parameter) {

  pinMode(LED1_PIN, OUTPUT);

  for (;;) {

    digitalWrite(LED1_PIN, HIGH);
    Serial.println("Task1: LED1 ON");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    digitalWrite(LED1_PIN, LOW);
    Serial.println("Task1: LED1 OFF");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.printf("Task1 Stack Free: %u bytes\n",
                  uxTaskGetStackHighWaterMark(NULL));
  }
}

// TASK 2 
void Task2(void *parameter) {

  pinMode(LED2_PIN, OUTPUT);

  for (;;) {

    digitalWrite(LED2_PIN, HIGH);
    Serial.println("Task2: LED2 ON");
    vTaskDelay(333 / portTICK_PERIOD_MS);

    digitalWrite(LED2_PIN, LOW);
    Serial.println("Task2: LED2 OFF");
    vTaskDelay(333 / portTICK_PERIOD_MS);

    Serial.printf("Task2 Stack Free: %u bytes\n",
                  uxTaskGetStackHighWaterMark(NULL));
  }
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.printf("Starting FreeRTOS\nInitial Free Heap: %u bytes\n",
                xPortGetFreeHeapSize());

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // CREATE TASK1
  xTaskCreatePinnedToCore(
    Task1,
    "Task1",
    10000,
    NULL,
    1,
    &Task1Handle,
    1
  );

  // CREATE TASK2
  xTaskCreatePinnedToCore(
    Task2,
    "Task2",
    10000,
    NULL,
    1,
    &Task2Handle,
    1
  );
}

void loop() {

  static uint32_t lastCheck = 0;

  if (millis() - lastCheck > 5000) {

    Serial.printf("Free Heap: %u bytes\n",
                  xPortGetFreeHeapSize());

    lastCheck = millis();
  }
}
