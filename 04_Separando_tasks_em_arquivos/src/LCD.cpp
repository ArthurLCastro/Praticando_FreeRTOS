#include "LCD.h"

void vTask_LCD(void * pvParameters) {
  int leitura;
  bool buttonState, lastButtonState= -1;

  lcd.init();
  lcd.backlight();

  while (1) {
    if(xQueueReceive(xQueue_ButtonClick, &buttonState, pdMS_TO_TICKS(10)) == pdTRUE){     
      if (buttonState != lastButtonState){
        lastButtonState = buttonState;

        Serial.println("[LCD] Estado do botão: " + String(buttonState));

        lcd.setCursor(0, 0);
        lcd.print("Button:         ");
        lcd.setCursor(7, 0);

        if (buttonState){
          lcd.print("Click!");
        } else {
          lcd.print("Solto");
        }
      }
    }

    if(xQueueReceive(xQueue_AnalogValue, &leitura, pdMS_TO_TICKS(10)) == pdTRUE){
      Serial.println("[LCD] Leitura Analógica: " + String(leitura));
  
      lcd.setCursor(0, 1);
      lcd.print("Lido:           ");
      lcd.setCursor(9, 1);
      lcd.print(leitura);
    }
  }  
}