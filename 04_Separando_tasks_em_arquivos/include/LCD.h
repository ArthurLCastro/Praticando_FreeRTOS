#ifndef LCD_h
#define LCD_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR      0x27

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
              
#endif