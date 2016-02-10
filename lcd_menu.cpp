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
    ts = 0;
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
    sub = (millis() - ts) / 4000;

    if (sub > 0 ){
        step_cur += sub;

        return step_cur;
    }

}

void wait::set_steps(uint16_t s){
    steps = s;
}


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

    wait w1, w2, w3, w4;


void lcd_target(LiquidCrystal_I2C lcd, unsigned int target, double distance){

    unsigned long ts;
    unsigned int step;
     w1.set_time(4000);
     w1.start();
     w1.set_steps(2);


    if(millis() - ts > 4000){
        step++;
        if (step > 5) step = 0;
    }


    switch (target){
    case 1:
        lcd.clear();
        if(w1.step() ==1){
            //lcd.print("OOOOOOOOOOOOOOOOHHHHHHHHHHHHHHHH");
            lcd.print("Find the monumen"); lcd.setCursor(0,1);
            lcd.print("t of Peasant");
            w2.start(4000);
        }
        //delay(4000);
        if(w1.step() == 2 ){
            lcd.clear();
            lcd.print("uprising that"); lcd.setCursor(0,1);
            lcd.print("overlook city.");
        }
        delay(4000);
        break;

    case 2: // Find the dragon above entrance to the castle.  When you find it Check your phone
        lcd.clear();
      //lcd.print("OOOOOOOOOOOOOOOOHHHHHHHHHHHHHHHH");
        lcd.print("Find the dragon"); lcd.setCursor(0,1);
        lcd.print("above entrance");
        delay(4000);
        lcd.clear();
        lcd.print("to the castle."); lcd.setCursor(0,1);
        lcd.print("When you find it");
        delay(4000);
        lcd.clear();
        lcd.print("Check your phone"); lcd.setCursor(0,1);
        break;
    case 3:
        break;


    }

}



