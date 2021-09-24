/********************************************************************************
  Praticando FreeRTOS
  Arthur Castro - Set/2021 
 *******************************************************************************/

#include <Arduino.h>

#include <freertos\FreeRTOS.h>
#include <freertos\task.h>

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

void vTask_HeartBeatLED(void * pvParameters);
void vTask_ButtonLED(void * pvParameters);
void vTask_AnalogInput(void * pvParameters);
void vTask_LCD(void * pvParameters);

void setup() {
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
  int valorLido;

  Serial.begin(115200);
  pinMode(POT_PIN, INPUT);

  while (1) {
    valorLido = analogRead(POT_PIN);
    Serial.println("Leitura Analógica: " + String(valorLido));
    
    vTaskDelay(pdMS_TO_TICKS(50));
  }  
}

void vTask_LCD(void * pvParameters) {
  int counter = 0;

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("--- FreeRTOS ---");
  lcd.setCursor(0, 1);
  lcd.print("Contador: ");

  while (1) {
    lcd.setCursor(10, 1);
    lcd.print(counter++);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }  
}