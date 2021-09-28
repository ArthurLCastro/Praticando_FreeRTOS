/********************************************************************************
  Praticando FreeRTOS
  Arthur Castro - Set/2021 

  Uso de semaforo para controle do recurso compartilhado de LCD entre tasks
  vTask_AnalogInput e vHandle_ButtonLED controlando o mesmo display LCD
 *******************************************************************************/

#include <Arduino.h>

#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include <freertos\semphr.h>

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
             vHandle_AnalogInput;

SemaphoreHandle_t xSemaphore_LCD;

void vTask_HeartBeatLED(void * pvParameters);
void vTask_ButtonLED(void * pvParameters);
void vTask_AnalogInput(void * pvParameters);

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Criacao de semaforo
  xSemaphore_LCD = xSemaphoreCreateMutex();

  if(xSemaphore_LCD == NULL){
    Serial.println("ERRO na criação do semáforo!");
    while(1);
  }

  // Criacao de tarefas
  xTaskCreate(vTask_HeartBeatLED, "HB_LED", configMINIMAL_STACK_SIZE+1024, NULL, 0, &vHandle_HeartBeatLED);
  xTaskCreate(vTask_ButtonLED, "BUT_LED", configMINIMAL_STACK_SIZE+1024, NULL, 1, &vHandle_ButtonLED);
  xTaskCreate(vTask_AnalogInput, "ANALOG_IN", configMINIMAL_STACK_SIZE+1024, NULL, 2, &vHandle_AnalogInput); 
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
  bool estadoBotao, estadoAnterior = -1;
  
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Botao:          ");

  while (1) {
    estadoBotao = !digitalRead(BUT_PIN);

    // if(estadoBotao){
    //   digitalWrite(LED2_PIN, HIGH);
    // } else {
    //   digitalWrite(LED2_PIN, LOW);
    // }

    if(estadoBotao != estadoAnterior){
      estadoAnterior = estadoBotao;

      xSemaphoreTake(xSemaphore_LCD, 0);

      lcd.setCursor(0, 0);
      lcd.print("Botao:          ");
      lcd.setCursor(7, 0);

      if (estadoBotao){
        lcd.print("Press!");
        digitalWrite(LED2_PIN, HIGH);
      } else {
        lcd.print("Norm");
        digitalWrite(LED2_PIN, LOW);
      }

      xSemaphoreGive(xSemaphore_LCD);
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
      xSemaphoreTake(xSemaphore_LCD, 0);
      
      lcd.setCursor(0, 1);
      lcd.print("Leitura:        ");
      lcd.setCursor(9, 1);
      lcd.print(valorLido);
      
      xSemaphoreGive(xSemaphore_LCD);
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }  
}