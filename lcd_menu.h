#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "LiquidCrystal_I2C.h"
#include <string.h>

//class lcd_target {
//public:
//    lcd_target();
//    LiquidCrystal_I2C lcd;
//    double lat, lon;
//    char text[256];
//    char *text_search;

//    void set_lcd(LiquidCrystal_I2C lcd1);

//private:
//    void display();
//    unsigned int text_srch_temp;

//};



void lcd_welcome(LiquidCrystal_I2C lcd);
void lcd_open_box(LiquidCrystal_I2C lcd);
void lcd_box_open(LiquidCrystal_I2C lcd);
void lcd_distance_target1(LiquidCrystal_I2C lcd, double distance);
void lcd_gps_signal(LiquidCrystal_I2C lcd, unsigned int strength);

void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance);



#endif // LCD_MENU_H

