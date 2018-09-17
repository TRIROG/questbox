#include "lcd_menu.h"
#include "QwestBox.h"
#include "wait.h"

#define SIGNAL_THRES 4

byte black_box[8] = {
    B00000,
    B10001,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000,
};

wait wait_4_steps, wait_3_steps, wait_0_steps, step;

void lcd_welcome(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("   Quest Box  "));

    float bat_voltage;

    for (int i = 0; i <= BATTERY_AVAREGE_COUNT; i++)
        bat_voltage = get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    float bat_percent = (bat_voltage - 3.3)*111.1; //Use 2.75 on first box

    if(bat_percent > 100) bat_percent = 100;
    if(bat_percent < 0 ) bat_percent = 0;

    lcd.setCursor(0,1);
    lcd.print(F("Batt: "));

    lcd.print((int)bat_percent);
    lcd.print("% ");
    lcd.print( bat_voltage);
    lcd.print("V   ");

    lcd.print((int)bat_percent);

    delay(4000);
}

void lcd_distance_target1(LiquidCrystal_I2C lcd, unsigned long distance){
    //lcd.clear();
    lcd.setCursor(0, 0);
#ifdef MULTIPLE_TRAGETS
    lcd.print(F("Your next stop"));
#else
    lcd.print(F("Your target"));
#endif
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
    lcd.setCursor(0, 0);
    lcd.print(F("   GPS signal   "));
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

#define STEP_1 wait_4_steps.step()

#ifdef MULTIPLE_TRAGETS
void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance, unsigned int signal){

    wait_4_steps.set_steps(4);
    wait_3_steps.set_steps(3);
    wait_0_steps.set_steps(3);

    static unsigned int wait_temp;
    if (wait_temp != wait_4_steps.step()) {
        lcd.clear();
        wait_temp = wait_4_steps.step();
    }

    int step3  = wait_3_steps.step();
    int step4  = wait_4_steps.step();


    switch (target){
    case 1: // Lutkovno Gledalsce
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }
        break;

    case 2: // Vhod na grad
        if(wait_4_steps.step() == 1){
            lcd.setCursor(0, 0);
            lcd.print(F("A dragon you")); lcd.setCursor(0,1);
            lcd.print(F("will meet,"));
        }
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

    case 3: // Grajska kapela
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }

        break;

    case 4: // Cerkev sv. Florjana
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }

        break;

    case 5: // Kip
        if(wait_4_steps.step() == 1){
            lcd.setCursor(0, 0);
            lcd.print(F("A house by the")); lcd.setCursor(0,1);
            lcd.print(F("church you"));
        }
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

    case 6: // Krakovski most
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }

        break;

    case 7: // Trg FR
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }
        break;

    case 8: // Napoleonov kip
        switch (wait_3_steps.step()) {
        case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("Find a")); lcd.setCursor(0,1);
            lcd.print(F("Golden face,"));
            break;
        case 2:
            lcd.setCursor(0, 0);
            lcd.print(F("Twenty steps")); lcd.setCursor(0,1);
            lcd.print(F("from this place."));
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
    case 9: // Zlata ladjica
        switch (wait_3_steps.step()){
        case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("Dragons at the")); lcd.setCursor(0,1);
            lcd.print(F("river wait,"));
            break;
        case 2:
            lcd.setCursor(0, 0);
            lcd.print(F("Lurking")); lcd.setCursor(0,1);
            lcd.print(F("for another bait"));
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

    case 10: // Sustarski most
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }
        break;

    case 11: // Presernov trg
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }
        break;

    case 12: // Rotovz
        if(wait_4_steps.step() == 1){
            lcd.setCursor(0, 0);
            lcd.print(F("To the fountain")); lcd.setCursor(0,1);
            lcd.print(F("you must go"));
        }
        if(wait_4_steps.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print(F("from witch")); lcd.setCursor(0,1);
            lcd.print(F("three rivers"));
        }
        if(wait_4_steps.step() == 3 ){
            lcd.setCursor(0, 0);
            lcd.print(F("flow      ")); lcd.setCursor(0,1);
            lcd.print(F("               "));
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

    case 13: // Stolnica
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }
        break;

    case 14: // Zmajski most
        if(wait_3_steps.step() ==1){
            lcd.setCursor(0, 0);
            lcd.print(F("Dragon bridge")); lcd.setCursor(0,1);
            lcd.print(F("is near, if you"));
        }
        if(wait_3_steps.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print(F("dont't find it")); lcd.setCursor(0,1);
            lcd.print(F("ask, do not fear."));
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

    case 15: // Mesarski most
        if(signal > SIGNAL_THRES){
            lcd_distance_target1(lcd, distance);
        }
        else {
            lcd_gps_signal(lcd, signal);
        }
        break;

    case 16: // Petkovsek
        switch (step4){
        case 1:
            lcd.setCursor(0, 0);
            lcd.print(F("Under the bridge")); lcd.setCursor(0,1);
            lcd.print(F("is a palm tree,"));
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

#else
void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance, unsigned int signal){

    static int _lcd_clr, _lcd_clear_tmp;

    // Clear lcd on change
    if (_lcd_clr != _lcd_clear_tmp) {
        lcd.clear();
        _lcd_clear_tmp = _lcd_clr;
    }

    if(signal > SIGNAL_THRES){
        lcd_distance_target1(lcd, distance);
        _lcd_clr = 1;
    }
    else {
        lcd_gps_signal(lcd, signal);
        _lcd_clr = 2;
    }

}

#endif






