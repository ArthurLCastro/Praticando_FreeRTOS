/********************************************************************************
  Praticando FreeRTOS
  Arthur Castro - Set/2021 

  Uso de fila para troca de dados entre tasks
  vTask_LCD recebe o valor lido pela vTask_AnalogInput e do vHandle_ButtonLED
  Dividindo projeto em arquivos diferentes para a implementacao de cada task
 *******************************************************************************/

#include "main.h"

void setup() {
  Serial.begin(115200);

  // Criacao de filas
  xQueue_AnalogValue = xQueueCreate(3, sizeof(int));
  xQueue_ButtonClick = xQueueCreate(1, sizeof(bool));

  if((xQueue_AnalogValue == NULL) || (xQueue_ButtonClick == NULL)){
    Serial.println("ERRO na criação de fila!");
    while(1);
  }

  // Criacao de tarefas
  xTaskCreate(vTask_HeartBeatLED, "HB_LED", configMINIMAL_STACK_SIZE+1024, NULL, 0, &vHandle_HeartBeatLED);
  xTaskCreate(vTask_ButtonLED, "BUT_LED", configMINIMAL_STACK_SIZE+1024, NULL, 1, &vHandle_ButtonLED);
  xTaskCreate(vTask_AnalogInput, "ANALOG_IN", configMINIMAL_STACK_SIZE+1024, NULL, 2, &vHandle_AnalogInput);
  xTaskCreate(vTask_LCD, "LCD", configMINIMAL_STACK_SIZE+1024, NULL, 1, &vHandle_LCD);  
}

void loop() {
  vTaskDelete(NULL);    // Exclui a task criada para o loop
}