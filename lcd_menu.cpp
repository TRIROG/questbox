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


//lcd_target::lcd_target(){
//    //lcd_target::lcd = lcd;
//}

//void lcd_target::set_lcd(LiquidCrystal_I2C lcd1){
//    lcd = lcd1;
//}


//void lcd_target::display(){

//    unsigned int text_pos_cur, text_pos_prev;
//    char page[5][256];
//    unsigned int cur_page = 0;

//    char *text_search;

//    //lcd.clear();

//    if (strlen(text) > 32){
//        text_search = strchr(text, ' ');

//        while (lcd_target::text_search != NULL)
//        {
//          text_pos_prev = lcd_target::text_search - lcd_target::text + 1; //current position of " ".

//          //printf ("found at %d\n",lcd_target::text_search-lcd_target::text+1);

//          lcd_target::text_search = strchr(lcd_target::text + 1, ' ');
//          text_pos_cur = lcd_target::text_search - lcd_target::text + 1;

//          if (text_pos_prev < 32 && text_pos_cur >= 32) {
//              strncpy(page[0], text, text_pos_prev);
//          }

////          if (text_pos_prev + 32 < 64 && text_pos_cur >= 32) {
////              strncpy(page[0], text)
////          }

//        }

//    }


////    lcd.setCursor(0,1);
////    lcd.print("Battery: ");
////    lcd.print((int)bat_percent);
//    lcd_target::lcd.print("%");
////    delay(3000);
//}


wait::wait(){
    //ts = 0;
}

int wait::start(){
    ts = millis();
}

void wait::set_time(unsigned long t){
    time = t;
}

int wait::end(){
    if(millis() - ts > time) return 1;
    else return 0;
}

int wait::start(unsigned long t){
    time = t;
    wait::start();
}

unsigned int wait::step(){
    uint16_t sub;
    sub = (millis() - wait::ts);

    //Serial.print("Sub: "); Serial.println(wait::ts);
    Serial.print("Sub: "); Serial.println(sub);
    Serial.print("ts: "); Serial.println(wait::ts);

    Serial.print("Sub/4000: "); Serial.println(sub/4000);

    if (sub > 4000 ){

        step_cur += sub/4000;

        Serial.print("Sub increment to: "); Serial.println(step_cur);

        wait::ts = millis();
        if(step_cur > steps) step_cur = 1;


        Serial.print("Step_cur: "); Serial.println(step_cur);
        return step_cur;
    }
    else return step_cur;

}

void wait::set_steps(uint16_t s){
    steps = s;
}


void lcd_welcome(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0, 0);
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
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your next stop");
    lcd.setCursor(0,1);
    lcd.print("is ");
    lcd.print((int)distance);
    lcd.print(" ");
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

    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   GPS signal");
    lcd.setCursor(0,1);
    lcd.print("[");
    lcd.print(bar);
    lcd.setCursor(15,1);
    lcd.print("]");
}



void lcd_open_box(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Opening box.");
    lcd.setCursor(0, 1);
    lcd.print(" Stand clear... ");
}

void lcd_box_open(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.print("  Box open ");
}

    wait wait_1, wait_2, wait_3, w4;


void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance, unsigned int signal){

    unsigned long ts;
    unsigned int step;
    // wait_1.set_time(4000);
     //wait_1.start();
     wait_1.set_steps(4);
     wait_2.set_steps(4);
     wait_3.set_steps(4);
   //  wait_4.set_steps(3);

    Serial.print("Target: ");
    Serial.print(target);
    Serial.print(", step: ");
    Serial.println(wait_1.step());

    static unsigned int wait_temp;
    if (wait_temp != wait_1.step()){
        lcd.clear();

        Serial.println("CLLLLLLLLLLLLLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEAAAAAAAAAAAAAAAAAAR");
        delay(300);

        wait_temp = wait_1.step();
    }


    switch (target){
    case 1:
        //lcd.clear();
        //lcd.setCursor(0, 0);
        //lcd.print("Test");
        //delay(2000);
        if(wait_1.step() ==1){
            lcd.setCursor(0, 0);
            lcd.print("A dragon you"); lcd.setCursor(0,1);
            lcd.print("will meet,");
            wait_2.start(4000);
        }
        //delay(4000);
        if(wait_1.step() == 2 ){
            lcd.setCursor(0, 0);
            lcd.print("above the"); lcd.setCursor(0,1);
            lcd.print("castle door");
        }
        if(wait_1.step() == 3 ){
            lcd.setCursor(0, 0);
            lcd.print("you will find"); lcd.setCursor(0,1);
            lcd.print("his seat.");
        }
        if(wait_1.step() == 4 ){
            if(signal > 5){
                lcd_distance_target1(lcd, distance);
            }
            else {
                lcd_gps_signal(lcd, signal);
            }
        }
        break;

    case 2: //
        lcd.clear();
        lcd.print("A dragon you"); lcd.setCursor(0,1);
        lcd.print("will meet,");
        delay(4000);
        lcd.clear();
        lcd.print("above the"); lcd.setCursor(0,1);
        lcd.print("castle door");
        delay(4000);
        lcd.clear();
        lcd.print("you will find"); lcd.setCursor(0,1);
        lcd.print("his seat.");
        break;
    case 3:
        lcd.clear();
        lcd.print("A house by the"); lcd.setCursor(0,1);
        lcd.print("church you");
        delay(4000);
        lcd.clear();
        lcd.print("should mind,"); lcd.setCursor(0,1);
        lcd.print("A statue with");
        delay(4000);
        lcd.clear();
        lcd.print("a dragon there"); lcd.setCursor(0,1);
        lcd.print("you will find.");
        break;


    }

}



