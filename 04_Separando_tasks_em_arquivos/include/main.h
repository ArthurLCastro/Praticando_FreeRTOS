#ifndef main_h
#define main_h

#include <Arduino.h>

#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include <freertos\queue.h>

// #include "AnalogInput.h"
// #include "ButtonLED.h"
// #include "HeartBeatLED.h"
// #include "LCD.h"

QueueHandle_t xQueue_AnalogValue,
              xQueue_ButtonClick;

TaskHandle_t vHandle_HeartBeatLED,
             vHandle_ButtonLED,
             vHandle_AnalogInput,
             vHandle_LCD;

void vTask_HeartBeatLED(void * pvParameters);
void vTask_ButtonLED(void * pvParameters);
void vTask_AnalogInput(void * pvParameters);
void vTask_LCD(void * pvParameters);

#endif