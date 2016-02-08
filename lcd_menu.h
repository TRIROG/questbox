#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "LiquidCrystal_I2C.h"

void lcd_welcome(LiquidCrystal_I2C lcd);
void lcd_open_box(LiquidCrystal_I2C lcd);
void lcd_box_open(LiquidCrystal_I2C lcd);
void lcd_distance_target1(LiquidCrystal_I2C lcd, double distance);
void lcd_gps_signal(LiquidCrystal_I2C lcd, unsigned int strength);



#endif // LCD_MENU_H

