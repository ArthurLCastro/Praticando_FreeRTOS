#include "AnalogInput.h"

void vTask_AnalogInput(void * pvParameters) {
  int valorLido, valorAnterior = -1;

  pinMode(POT_PIN, INPUT);

  while (1) {
    valorLido = analogRead(POT_PIN);

    if(valorLido != valorAnterior){       // Envia o valor apenas se for diferente do ultimo lido
      valorAnterior = valorLido;
  
      xQueueSend(xQueue_AnalogValue, &valorLido, portMAX_DELAY);
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }  
}