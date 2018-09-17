#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "LiquidCrystal_I2C.h"
#include <string.h>


void lcd_welcome(LiquidCrystal_I2C lcd);
void lcd_open_box(LiquidCrystal_I2C lcd);
void lcd_box_open(LiquidCrystal_I2C lcd);
void lcd_distance_target1(LiquidCrystal_I2C lcd, unsigned long distance);
void lcd_gps_signal(LiquidCrystal_I2C lcd, unsigned int strength);

void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance, unsigned int signal);



#endif // LCD_MENU_H

