
#include "QwestBox.h"
#include "LiquidCrystal_I2C.h"
#include "TinyGPS++.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include "lcd_menu.h"
#include "EEPROM.h"

TinyGPSPlus gps;
//LiquidCrystal lcd(8, 10, 4, 5, 6, 7);
LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial ss(4, 5);
Servo servo;

void secret_button();
int distance2();
void open_box();
void go_sleep(void);

unsigned long lcd_time;
unsigned long update_time = 0;
unsigned long sleep_time = 0;
double position_lat, position_lon;
double d, distance;
int sats_view, sats_fix, gps_fix;
long hdop;
bool newData = false;
unsigned long chars;
unsigned short sentences, failed;
int but;
int test = 0;
bool interrupt_flag = 0;
bool fix = 0;
int strength;
unsigned int target;






void setup()
{
    //lcd_target target1_zmajev_tempelj;

//    target1_zmajev_tempelj.set_lcd(lcd);
//    target1_zmajev_tempelj.lat = 46.047575;
//    target1_zmajev_tempelj.lon = 14.505386;
    //target1_zmajev_tempelj.text = "Find the monument of the Peasant uprisings that overlook the city.";


    //EEPROM.write(1,1);

    // Init pin outputs
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SERVO_ON_PIN, OUTPUT);
    pinMode(ON_PIN, INPUT);
    pinMode(LCD_POWER_PIN, OUTPUT);

    digitalWrite(LCD_POWER_PIN, 1);
    digitalWrite(SERVO_ON_PIN, 0);

    ss.begin(9600);
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();

    lcd_welcome(lcd);
    //lcd.print("   Quest Box  ");

    Serial.println("Setup done.");
}

void loop()
{
    // Check secret button
    but = !digitalRead(ON_PIN);
    if(but){
        secret_button();
    }

    unsigned long t = millis();
    while (ss.available() > 0 || t + 1100 < millis())
        gps.encode(ss.read());

    sats_fix = gps.satellites.value();
    hdop = gps.hdop.value();
    distance = gps.distanceBetween(position_lat, position_lon, TARGET_1_LAT, TARGET_1_LON);
    position_lat = gps.location.lat();
    position_lon = gps.location.lng();



    if(millis() > update_time + 1000){
        update_time = millis();

        Serial.println(gps.location.lat(), 6);
        Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
        Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
        Serial.print("ALT=");  Serial.println(gps.altitude.meters());
        Serial.print("hdop=");  Serial.println(hdop);
        Serial.print("sats=");  Serial.println(sats_fix);

        if(hdop < 500 && sats_fix > 3){
            fix = 1;
            distance = gps.distanceBetween(position_lat, position_lon, TARGET_1_LAT, TARGET_1_LON);
            lcd_distance_target1(lcd, distance);
        }
        else{
            fix = 0;
            strength = sats_fix;
            lcd_gps_signal(lcd, strength);
        }

        if(distance < 50 && distance != 0){
            open_box();
        }
    }



    target = 1;
    lcd_target(lcd, target, distance);

    if (fix){
        lcd_distance_target1(lcd, distance);
    }
    else{
        lcd_gps_signal(lcd, strength);
        delay(4000);
    }



    if(millis() > sleep_time + SLEEP_TIME_MS){
        lcd.clear();
        //lcd.print("    No GPS");
        //lcd.setCursor(0, 1);
        lcd.print("Going to sleep..");
        delay(2000);

        go_sleep();
    }
}

void go_sleep(void)
{
    pinMode(ON_PIN, OUTPUT);
    digitalWrite(ON_PIN, 0);
}


int step1()
{
    //Wait for GPS
    lcd.clear();
    lcd.println("Looking for GPS");
    lcd.setCursor(0, 1);
    char str[32];
    sprintf(str,"view:%2d  fix:%2d", sats_view, sats_fix);
    lcd.print(str);

    delay(1000);

    if(millis() > sleep_time + 300000) digitalWrite(ON_PIN, 0);
    if(gps_fix == 3) return 1;
    else return 0;

}

void open_box(){
    lcd_open_box(lcd);
    delay(4000);
    servo.attach(9);
    servo.attach(SERVO_PIN);
    digitalWrite(SERVO_ON_PIN, 1);
    servo.write(DOOR_OPEN);
    lcd_box_open(lcd);
    lcd.clear();
    lcd.print("    Box open ");
    delay(20000);
    servo.write(DOOR_CLOSED);
    delay(3000);
    servo.detach();
}


void secret_button()
{
    static unsigned long but1;
    Serial.println("1");
    while(!digitalRead(ON_PIN)){delay(20);}
    but1 = millis();
    while(millis() < but1 + BUTTON1_DELAY){delay(20);
        if(!digitalRead(ON_PIN)){
            Serial.println("2");
            while(!digitalRead(ON_PIN)){delay(20);}
            but1 = millis();
            while(millis() < but1 + BUTTON1_DELAY){delay(20);
                if(!digitalRead(ON_PIN)){
                    Serial.println("3");
                    while(!digitalRead(ON_PIN)){delay(20);}
                    but1 = millis();
                    while(millis() < but1 + BUTTON1_DELAY){delay(20);
                        if(!digitalRead(ON_PIN)){
                            Serial.println("4");
                            while(!digitalRead(ON_PIN)){delay(20);}
                            but = millis();
                            while(millis() < but1 + BUTTON1_DELAY){delay(20);
                                if(!digitalRead(ON_PIN)){
                                    Serial.println("5");
                                    but1 = millis();
                                    while(!digitalRead(ON_PIN)){delay(20);
                                        Serial.println("6");
                                        if(millis() > but1 + 1000){
                                            open_box();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


float get_battery_voltage_avg(float bat_read){
    float volt = bat_read*5/1023;
    float bat_volt = 0;
    static float volt_arr[BATTERY_AVAREGE_COUNT];
    static int volt_cursor;

    volt_arr[volt_cursor] = volt;
    if(volt_cursor >= BATTERY_AVAREGE_COUNT-1) volt_cursor = 0;
    else volt_cursor++;


    for(int i = 0; i < BATTERY_AVAREGE_COUNT; i++){
        bat_volt += volt_arr[i];
    }
    bat_volt = bat_volt/BATTERY_AVAREGE_COUNT;

    float bat_percent = (bat_volt - 3.3)*111.1;


    Serial.print("Volt/curs: ");
    Serial.println(volt);
    Serial.print("/");
    Serial.println(volt_cursor);
    Serial.println(bat_read);
    Serial.println(bat_percent);


    return bat_volt;
}

