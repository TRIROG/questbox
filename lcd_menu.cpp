#include "lcd_menu.h"
#include "QwestBox.h"
#include "wait.h"


#define SIGNAL_THRES 5

byte black_box[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};


void lcd_welcome(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("   Quest Box  "));

    // Init battery voltage
    for(int i = 0; i < BATTERY_AVAREGE_COUNT; i++) get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    float bat_voltage = get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    float bat_percent = (bat_voltage - 2.75)*111.1; //Shoukd be 3.3 but there is some voltage drop. It is temporary

    lcd.setCursor(0,1);
    lcd.print(F("Battery: "));
    lcd.print((int)bat_percent);
    lcd.print(F("%"));
    delay(3000);
}

void lcd_distance_target1(LiquidCrystal_I2C lcd, unsigned long distance){
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Your next stop"));
    lcd.setCursor(0,1);
    lcd.print(F("is "));
    lcd.print(distance);
    lcd.print(F(" "));
    lcd.print(F("m away..."));
}

void lcd_gps_signal(LiquidCrystal_I2C lcd, unsigned int strength){
    String bar;
    lcd.createChar(0, black_box);
    if (strength > 10) strength = 10;

    for(unsigned int i = 0; i < strength; i++){
        bar.concat("|");
        //lcd.write(byte(0));
    }

    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("   GPS signal"));
    lcd.setCursor(0,1);
    lcd.print(F("["));
    lcd.print(bar);
    lcd.setCursor(15,1);
    lcd.print(F("]"));
}



void lcd_open_box(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("  Opening box."));
    lcd.setCursor(0, 1);
    lcd.print(F(" Stand clear... "));
}

void lcd_box_open(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.print(F("    Box open "));
}

    wait wait_4_steps, wait_3_steps, wait_3, w4;


void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance, unsigned int signal){

    unsigned long ts;
    unsigned int step;

     wait_4_steps.set_steps(4);
     wait_3_steps.set_steps(3);
     wait_3.set_steps(4);
   //  wait_4.set_steps(3);

    static unsigned int wait_temp;
    if (wait_temp != wait_4_steps.step()){
        lcd.clear();
        wait_temp = wait_4_steps.step();
    }

    int step3  = wait_3_steps.step();
    int step4  = wait_4_steps.step();


    switch (target){
    case 1:
        if(wait_4_steps.step() ==1){
            lcd.setCursor(0, 0);
            lcd.print(F("A dragon you")); lcd.setCursor(0,1);
            lcd.print(F("will meet,"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
        }
        //delay(SCREEN_DELAY_DEFAULT);
        if(wait_4_steps.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print(F("above the")); lcd.setCursor(0,1);
            lcd.print(F("castle door"));
        }
        if(wait_4_steps.step() == 3 ){
            lcd.setCursor(0, 0);
            lcd.print(F("you will find")); lcd.setCursor(0,1);
            lcd.print(F("his seat."));
        }
        if(wait_4_steps.step() == 4 ){
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
        }
        break;

    case 2: //
        if(wait_4_steps.step() ==1){
            lcd.setCursor(0, 0);
            lcd.print(F("A house by the")); lcd.setCursor(0,1);
            lcd.print(F("church you"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
        }
        //delay(SCREEN_DELAY_DEFAULT);
        if(wait_4_steps.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print(F("should mind,")); lcd.setCursor(0,1);
            lcd.print(F("A statue with"));
        }
        if(wait_4_steps.step() == 3 ){
            lcd.setCursor(0, 0);
            lcd.print(F("a dragon there")); lcd.setCursor(0,1);
            lcd.print(F("you will find."));
        }
        if(wait_4_steps.step() == 4 ){
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
        }
        break;
    case 3:
        if(wait_3_steps.step() ==1){
            lcd.setCursor(0, 0);
            lcd.print(F("Find a")); lcd.setCursor(0,1);
            lcd.print(F("golden face,"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
        }
        //delay(SCREEN_DELAY_DEFAULT);
        if(wait_3_steps.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print(F("Twenty steps")); lcd.setCursor(0,1);
            lcd.print(F("from this place."));
        }
        if(wait_3_steps.step() == 3 ){
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
        }
        break;
    case 4:
        if(wait_3_steps.step() ==1){
            lcd.setCursor(0, 0);
            lcd.print(F("Dragons at the")); lcd.setCursor(0,1);
            lcd.print(F("river wait,"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
        }
        if(wait_3_steps.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print(F("lurking for")); lcd.setCursor(0,1);
            lcd.print(F("another bait."));
        }
        if(wait_3_steps.step() == 3 ){
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
        }
        break;
    case 5:
        switch (step4){
        case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("To the fountain")); lcd.setCursor(0,1);
            lcd.print(F("you must go"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
            break;
        case 2:
            lcd.setCursor(0, 0);
            lcd.print(F("from which")); lcd.setCursor(0,1);
            lcd.print(F("three rivers"));
            break;
        case 3:
            lcd.setCursor(0, 0);
            lcd.print(F("flow.")); lcd.setCursor(0,1);
            lcd.print(F(""));
            break;
        case 4:
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
            break;
        }
        break;
    case 6:
        switch (step3){
        case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("Dragon bridge")); lcd.setCursor(0,1);
            lcd.print(F("is near, If you"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
            break;
        case 2:
            lcd.setCursor(0, 0);
            lcd.print(F("don’t find it,")); lcd.setCursor(0,1);
            lcd.print(F("ask, do not fear"));
            break;
        case 3:
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
            break;
        }
        break;
    case 7:
        switch (step4){
        case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("Under the bridge")); lcd.setCursor(0,1);
            lcd.print(F("is a palm tree,"));
            wait_3_steps.start(SCREEN_DELAY_DEFAULT);
            break;
        case 2:
            lcd.setCursor(0, 0);
            lcd.print(F("there you")); lcd.setCursor(0,1);
            lcd.print(F("will find"));
            break;
        case 3:
            lcd.setCursor(0, 0);
            lcd.print(F("numbers three.")); lcd.setCursor(0,1);
            lcd.print(F(" "));
            break;
        case 4:
            if(signal > SIGNAL_THRES){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
            break;
        }
        break;
    }

}





