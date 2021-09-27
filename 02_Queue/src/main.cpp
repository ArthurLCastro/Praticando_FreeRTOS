/********************************************************************************
  Praticando FreeRTOS
  Arthur Castro - Set/2021 

  Uso de fila para troca de dados entre tasks
  vTask_LCD recebe o valor lido pela vTask_AnalogInput
 *******************************************************************************/

#include <Arduino.h>

#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include <freertos\queue.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED1_PIN      13
#define LED2_PIN      14
#define BUT_PIN       21
#define POT_PIN       A5

#define LCD_ADDR      0x27

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

TaskHandle_t vHandle_HeartBeatLED,
             vHandle_ButtonLED,
             vHandle_AnalogInput,
             vHandle_LCD;

QueueHandle_t xQueue_AnalogValue;

void vTask_HeartBeatLED(void * pvParameters);
void vTask_ButtonLED(void * pvParameters);
void vTask_AnalogInput(void * pvParameters);
void vTask_LCD(void * pvParameters);

void setup() {
  Serial.begin(115200);

  // Criacao de filas
  xQueue_AnalogValue = xQueueCreate(3, sizeof(int));

  if(xQueue_AnalogValue == NULL){
    Serial.println("ERRO: Não foi possível criar a fila!");
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

void vTask_HeartBeatLED(void * pvParameters) {
  pinMode(LED1_PIN, OUTPUT);

  while (1) {
    digitalWrite(LED1_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(300));
    digitalWrite(LED1_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(700));
  }  
}

void vTask_ButtonLED(void * pvParameters) {
  bool estadoBotao;

  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT_PULLUP);

  while (1) {
    estadoBotao = digitalRead(BUT_PIN);

    if(!estadoBotao){
      digitalWrite(LED2_PIN, HIGH);
    } else {
      digitalWrite(LED2_PIN, LOW);
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }  
}

void vTask_AnalogInput(void * pvParameters) {
  int valorLido, valorAnterior = -1;

  pinMode(POT_PIN, INPUT);

  while (1) {
    valorLido = analogRead(POT_PIN);
    
    if(valorLido != valorAnterior){       // Envia o valor apenas se for diferente do ultimo lido
      valorAnterior = valorLido;
  
      // Serial.println("[ADC] Leitura Analógica: " + String(valorLido));
      xQueueSend(xQueue_AnalogValue, &valorLido, portMAX_DELAY);
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }  
}

void vTask_LCD(void * pvParameters) {
  int leitura;

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("--- FreeRTOS ---");
  
  while (1) {
    if(xQueueReceive(xQueue_AnalogValue, &leitura, pdMS_TO_TICKS(1000)) == pdTRUE){
      Serial.println("[LCD] Leitura Analógica: " + String(leitura));
      Serial.println("----------------------------------------");
  
      lcd.setCursor(0, 1);
      lcd.print("Leitura:        ");
      lcd.setCursor(9, 1);
      lcd.print(leitura);
    }
  }  
}