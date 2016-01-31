
#include "QwestBox.h"
#include "LiquidCrystal_I2C.h"
#include "TinyGPS++.h"
#include <SoftwareSerial.h>
#include <Servo.h>

#define TARGET_1_LAT 46.080686
#define TARGET_1_LON 14.550043

#define LCD_BACKLIGHT_TIME 3000
#define LCD_POWER_PIN      3
#define ON_PIN             8
#define LED_PIN            13
#define BUTTON_PIN         2
#define SLEEP_TIME_MS      600000
#define BUTTON1_DELAY      1000
#define DOOR_CLOSED        110
#define DOOR_OPEN          50
#define SERVO_PIN          9
#define SERVO_ON_PIN       6
#define BATTERY_VOLTAGE_PIN A1
#define BATTERY_AVAREGE_COUNT 20

TinyGPSPlus gps;
//LiquidCrystal lcd(8, 10, 4, 5, 6, 7);
LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial ss(4, 5);
Servo servo;

void secret_button();
void digitalToggle(int P);
int distance2();
void open_box();
void go_sleep(void);
float get_battery_voltage_avg(float bat_read);

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
float bat_voltage;


void setup()
{
    // Init pin outputs
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SERVO_ON_PIN, OUTPUT);
    pinMode(ON_PIN, INPUT);
    pinMode(LCD_POWER_PIN, OUTPUT);


    // LCD power ON
    digitalWrite(LCD_POWER_PIN, 1);

    ss.begin(9600);

    Serial.begin(115200);
    Serial.println("QuestBox");

    lcd.init();
    lcd.backlight();
    lcd.print("   Quest Box  ");

    //servo.write(DOOR_CLOSED);

    digitalWrite(SERVO_ON_PIN, 0);

    // Init battery voltage
    for(int i = 0; i < BATTERY_AVAREGE_COUNT; i++) get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    bat_voltage = get_battery_voltage_avg(analogRead(BATTERY_VOLTAGE_PIN));
    float bat_percent = (bat_voltage - 3.3)*111.1;

    lcd.setCursor(0,1);
    lcd.print("Battery: ");
    lcd.print((int)bat_percent);
    lcd.print("%");
    delay(3000);

    Serial.println("Setup done.");
}

void loop()
{
    // Check secret button
    but = !digitalRead(ON_PIN);
    if(but){
        secret_button();
    }

    while (ss.available() > 0)
       gps.encode(ss.read());


        sats_fix = gps.satellites.value();
        hdop = gps.hdop.value();
        distance = gps.distanceBetween(position_lat, position_lon, TARGET_1_LAT, TARGET_1_LON);
        position_lat = gps.location.lat();
        position_lon = gps.location.lng();


//    while (ss.available() > 0)
//       gps.encode(ss.read());

//       sats_fix = gps.satellites.value();
//       hdop = gps.hdop.value();
//       distance = gps.distanceBetween(lat, lon, lat2, lon2);
//       lat = gps.location.lat();
//       lon = gps.location.lng();





    if(millis() > update_time + 1000){
        update_time = millis();

        Serial.println(gps.location.lat(), 6);

//        Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
//        Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
//        Serial.print("ALT=");  Serial.println(gps.altitude.meters());
//        Serial.print("hdop=");  Serial.println(gps.hdop.value());
//        Serial.print("sats=");  Serial.println(gps.satellites.value());


        Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
        Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
        Serial.print("ALT=");  Serial.println(gps.altitude.meters());
        Serial.print("hdop=");  Serial.println(hdop);
        Serial.print("sats=");  Serial.println(sats_fix);



        //Serial.print("Sats in view = "); Serial.println(sats_view);
        //Serial.print("Sats fix = "); Serial.println(sats_fix);
        //Serial.print("GPS fix = "); Serial.println(gps_fix);
        //Serial.print("lon = "); Serial.println(lon);
        //Serial.print("position_lat = "); Serial.println(position_lat);
        //Serial.print("distance is "); Serial.print(d); Serial.println(" m");


        if(hdop < 500 && sats_fix > 3){
            distance2();
            lcd.clear();
            lcd.print("Your goal is");
            lcd.setCursor(0,1);
            lcd.print(distance);
            lcd.print("m away...");
            //delay(8000);
        }
        else{
            lcd.clear();
            lcd.print("   GPS signal");
            lcd.setCursor(0, 1);
            lcd.print("fix:"); lcd.print(sats_fix);
            lcd.setCursor(8, 1);
            lcd.print("h:"); lcd.print(hdop);
        }

        if(distance < 50 && distance != 0){
            open_box();
        }
    }

    if(millis() > sleep_time + SLEEP_TIME_MS){
        lcd.clear();
        //lcd.print("    No GPS");
        //lcd.setCursor(0, 1);
        lcd.print("Going to sleep..");
        delay(2000);

        go_sleep();
    }


   // delay(500);

}


void digitalToggle(int P)
{
	if(digitalRead(P) == 1) digitalWrite(P, 0);
	else digitalWrite(P, 1);
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

int distance2()
{
    if(position_lat != 0 && position_lon != 0){
        double deg_to_rad =  3.14/180;

        double R = 6371000; // metres
        double fi1 = position_lat * deg_to_rad;
        double fi2 = TARGET_1_LAT  * deg_to_rad;
        double delta_fi = (TARGET_1_LAT-position_lat) * deg_to_rad;
        double delta_lambda = (TARGET_1_LON-position_lon) * deg_to_rad;

        double a = sin(delta_fi/2) * sin(delta_fi/2) +
                cos(fi1) * cos(fi2) *
                sin(delta_lambda/2) * sin(delta_lambda/2);

        d = R * ( 2 * atan2(sqrt(a), sqrt(1-a)) );
        return d;
    }
    else return -1;
}

void open_box(){
    lcd.clear();
    lcd.print("  Opening box.");
    lcd.setCursor(0, 1);
    lcd.print(" Stand clear... ");
    delay(4000);
    //servo.attach(9);
    servo.attach(SERVO_PIN);
    digitalWrite(SERVO_ON_PIN, 1);
    servo.write(DOOR_OPEN);
    lcd.clear();
    lcd.print("  Box open ");
    delay(10000);
    servo.write(DOOR_CLOSED);
    delay(2000);
    servo.detach();
}


void secret_button()
{
    static unsigned long but1, but2;
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
    float bat_voltage = 0;
    static float volt_arr[BATTERY_AVAREGE_COUNT];
    static int volt_cursor;

    volt_arr[volt_cursor] = volt;
    if(volt_cursor >= BATTERY_AVAREGE_COUNT-1) volt_cursor = 0;
    else volt_cursor++;


    for(int i = 0; i < BATTERY_AVAREGE_COUNT; i++){
        bat_voltage += volt_arr[i];
    }
    bat_voltage = bat_voltage/BATTERY_AVAREGE_COUNT;

    float bat_percent = (bat_voltage - 3.3)*111.1;


    Serial.print("Volt/curs: ");
    Serial.println(volt);
    Serial.print("/");
    Serial.println(volt_cursor);
    Serial.println(bat_read);


    return bat_voltage;
}

