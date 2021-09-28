#include "HeartBeatLED.h"

void vTask_HeartBeatLED(void * pvParameters) {
  pinMode(LED1_PIN, OUTPUT);

  while (1) {
    digitalWrite(LED1_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(300));
    digitalWrite(LED1_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(700));
  }  
}