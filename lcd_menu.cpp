#include "lcd_menu.h"
#include "QwestBox.h"

byte black_box[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};


class lcd_step_point(){

} step_point;




void lcd_welcome(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.print("   Quest Box  ");

    // Init battery voltage
    for(int i = 0; i < BATTERY_AVAREGE_COUNT; i++) get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    float bat_voltage = get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    float bat_percent = (bat_voltage - 3.3)*111.1;

    lcd.setCursor(0,1);
    lcd.print("Battery: ");
    lcd.print((int)bat_percent);
    lcd.print("%");
    delay(3000);
}

void lcd_distance_target1(LiquidCrystal_I2C lcd, double distance){
    lcd.clear();
    lcd.print("Your goal is");
    lcd.setCursor(0,1);
    lcd.print(distance);
    lcd.print("m away...");
}

void lcd_gps_signal(LiquidCrystal_I2C lcd, unsigned int strength){
    String bar;
    lcd.createChar(0, black_box);
    if (strength > 10) strength = 10;

    for(unsigned int i = 0; i < strength; i++){
        bar.concat("|");
        //lcd.write(byte(0));
    }

    lcd.clear();
    lcd.print("   GPS signal");
    lcd.setCursor(0,1);
    lcd.print("[");
    lcd.print(bar);
    lcd.setCursor(15,1);
    lcd.print("]");
}



void lcd_open_box(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.print("  Opening box.");
    lcd.setCursor(0, 1);
    lcd.print(" Stand clear... ");
}

void lcd_box_open(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.print("  Box open ");
}
