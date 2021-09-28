#include "ButtonLED.h"

void vTask_ButtonLED(void * pvParameters) {
  bool estadoBotao;
  
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT_PULLUP);

  while (1) {
    estadoBotao = !digitalRead(BUT_PIN);

    if(estadoBotao){
      digitalWrite(LED2_PIN, HIGH);
    } else {
      digitalWrite(LED2_PIN, LOW);
    }

    xQueueSend(xQueue_ButtonClick, &estadoBotao, portMAX_DELAY);
    // xQueueOverwrite(xQueue_ButtonClick, &estadoBotao);

    vTaskDelay(pdMS_TO_TICKS(10));
  }  
}