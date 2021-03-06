
#include "QwestBox.h"
#include "LiquidCrystal_I2C.h"
#include "TinyGPS++.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include <PWMServo.h>
#include "lcd_menu.h"
#include "EEPROM.h"
#include "wait.h"
//#include "/home/vid/Arduino/libraries/MemoryFree/MemoryFree.h"

//#define SIMULATION
//#define BUTTON_FOR_NEXT_STEP

TinyGPSPlus gps;
//LiquidCrystal lcd(8, 10, 4, 5, 6, 7);
LiquidCrystal_I2C lcd(0x3F,16,2); // First box is 0x27, others 0x3F
SoftwareSerial ss(4, 5);

//Servo servo;
PWMServo servo;

void secret_button();
void open_box(bool close);
void close_box();
void go_sleep(void);
int distance2(double lat1, double lon1, double lat2, double lon2, unsigned long *dist);
void serial_receive();


unsigned long lcd_time;
unsigned long update_time = 0;
unsigned long sleep_time = 0;
double position_lat, position_lon;
double d;
unsigned long distance;
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

double target_lon[20];
double target_lat[20];
double single_target_lat;
double single_target_lon;
short radius;

extern int __heap_start, *__brkval;

int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void setup()
{
    target_lat[1] = TARGET_1_LAT;
    target_lon[1] = TARGET_1_LON;
    target_lat[2] = TARGET_2_LAT;
    target_lon[2] = TARGET_2_LON;
    target_lat[3] = TARGET_3_LAT;
    target_lon[3] = TARGET_3_LON;
    target_lat[4] = TARGET_4_LAT;
    target_lon[4] = TARGET_4_LON;
    target_lat[5] = TARGET_5_LAT;
    target_lon[5] = TARGET_5_LON;
    target_lat[6] = TARGET_6_LAT;
    target_lon[6] = TARGET_6_LON;
    target_lat[7] = TARGET_7_LAT;
    target_lon[7] = TARGET_7_LON;
    target_lat[8] = TARGET_8_LAT;
    target_lon[8] = TARGET_8_LON;
    target_lat[9] = TARGET_9_LAT;
    target_lon[9] = TARGET_9_LON;
    target_lat[10] = TARGET_10_LAT;
    target_lon[10] = TARGET_10_LON;
    target_lat[11] = TARGET_11_LAT;
    target_lon[11] = TARGET_11_LON;
    target_lat[12] = TARGET_12_LAT;
    target_lon[12] = TARGET_12_LON;
    target_lat[13] = TARGET_13_LAT;
    target_lon[13] = TARGET_13_LON;
    target_lat[14] = TARGET_14_LAT;
    target_lon[14] = TARGET_14_LON;
    target_lat[15] = TARGET_15_LAT;
    target_lon[15] = TARGET_15_LON;
    target_lat[16] = TARGET_16_LAT;
    target_lon[16] = TARGET_16_LON;

    ss.begin(9600);
    Serial.begin(57600);

#ifndef MULTIPLE_TRAGETS
    EEPROM.get(EEPROM_INDEX_LAT, single_target_lat);
    EEPROM.get(EEPROM_INDEX_LON, single_target_lon);
    EEPROM.get(EEPROM_INDEX_RAD, radius);
    Serial.print(F("Lattitude: ")); Serial.println(single_target_lat, 6);
    Serial.print(F("Longiude: ")); Serial.println(single_target_lon, 6);
    Serial.print(F("Radius: ")); Serial.println(radius);
#endif

    target = EEPROM.read(EEPROM_TARGET_INDEX);
    if((target > NUMBER_OF_TARGETS+2 || target == 0 ) && target != 255/*|| TARGET_RESET > 0*/) {
        target = 1;
        EEPROM.write(EEPROM_TARGET_INDEX, target);
    }

    // Force next step on strtup
    //target++;
    //EEPROM.write(EEPROM_TARGET_INDEX, target);

    // Init pin outputs
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SERVO_ON_PIN, OUTPUT);
    pinMode(ON_PIN, INPUT);
    pinMode(LCD_POWER_PIN, OUTPUT);
    pinMode(GPS_RX_PIN, INPUT);

    digitalWrite(LCD_POWER_PIN, 1);
    digitalWrite(SERVO_ON_PIN, 0);

    servo.attach(SERVO_PIN);

    analogReference(EXTERNAL);

    lcd.init();
    lcd.backlight();

    lcd_welcome(lcd);

    wait w;
    w.set_time(3000);
    w.start();

    wait sim;
    w.set_time(30000);
    sim.set_steps(7);
    sim.start();

    Serial.print(F("Free RAM:"));    Serial.println(freeRam());
    Serial.println(F("Setup done."));

    lcd.clear();
}

void loop()
{
    serial_receive();
    //Serial.print(F("Free RAM1:"));    Serial.println(freeRam());

    // Check secret button
    but = !digitalRead(ON_PIN);
    if(but){
        Serial.println(F("Button pressed"));
        secret_button();
    }
#ifdef MULTIPLE_TARGETS
    if (target == 255) {
        lcd.clear();
        lcd.print("Please return");
        lcd.setCursor(0,1);
        lcd.print("the box.");

        unsigned long ts = millis();
        while (ts + 60000 > millis()) {
            secret_button();
        }
    }
#endif
    // Check GPS
    bool GPS_debug = 0;
    unsigned long t = millis();
    while (ss.available() > 0 || t + 1100 < millis()){
        gps.encode(ss.read());
        GPS_debug = 1;
    }
    if (GPS_debug) {
        //Serial.println(F("GPS received"));
        GPS_debug = 0;
    }

    sats_fix = gps.satellites.value();
    hdop = gps.hdop.value();
    position_lat = gps.location.lat();
    position_lon = gps.location.lng();

    //target = 3; // debug
#ifdef MULTIPLE_TRAGETS
        distance = gps.distanceBetween(position_lat, position_lon, target_lat[target], target_lon[target]);
#else
    distance = gps.distanceBetween(position_lat, position_lon, single_target_lat, single_target_lon);
#endif
    //distance = 4; // debug

    static int sim, ok;
#ifdef SIMULATION
    if (but == 1){
        sim = 1;
        delay (1000);
    }
    else {
        sim = 0;
        lcd.setCursor(14, 1);
        lcd.print(target);
    }

    if (distance > 30 && sim == 0){
#else
#ifdef MULTIPLE_TRAGETS
    Serial.print(F("Distance: ")); Serial.println(distance);
    if (distance > 30) {
#else
    if (distance > radius) {
#endif
#endif
        lcd_target(lcd, target, distance, sats_fix);
    }
    else {

#ifdef BUTTON_FOR_NEXT_STEP
        wait finish;
        finish.set_time(2500);
        finish.set_steps(2);
        lcd.setCursor(0, 0);

        if(finish.step() == 1){
            lcd.print(F("You are at your "));
            lcd.setCursor(0, 1);
            lcd.print(F("current target."));
        }
        if (finish.step() == 2){
            lcd.setCursor(0, 0);
            lcd.print(F("Press the button"));
            lcd.setCursor(0, 1);
            lcd.print(F("to continue...  "));
        }

        if (but == 1){
            // Go to next target
            target++;
            ok = 1;



        }
#else
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("You are at your "));
        lcd.setCursor(0, 1);
        lcd.print(F("current target."));
        target++;
        delay(5000);
        but = 0;
#endif
#ifdef MULTIPLE_TRAGETS
        // Open the box at the end of game
        if(target > NUMBER_OF_TARGETS){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(F("CONGRATULATIONS! "));
            EEPROM.write(EEPROM_TARGET_INDEX, 255);
            delay(5000);
            open_box(DONT_LOCK);
        }
        else {
            static unsigned int target_temp;
            if(target_temp != target){
                EEPROM.write(EEPROM_TARGET_INDEX, target);
                target_temp = target;
            }
        }
#else
        if (distance < radius) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(F("CONGRATULATIONS! "));
            delay(5000);
            open_box(0);
        }
#endif

    }

    if(millis() > update_time + 1000){
        update_time = millis();
#ifdef DEBUG
        Serial.print(F("LAT="));  Serial.println(gps.location.lat(), 6);
        Serial.print(F("LONG=")); Serial.println(gps.location.lng(), 6);
        Serial.print(F("ALT="));  Serial.println(gps.altitude.meters());
        Serial.print(F("hdop="));  Serial.println(hdop);
        Serial.print(F("sats="));  Serial.println(sats_fix);
#endif
        if(hdop < 500 && sats_fix > 4 && distance < 1000000){
            fix = 1;
        }
        else if (hdop < 500 && sats_fix > 4) {
            Serial.println(F("Bad measure. GPS dump:"));
            Serial.print(F("LAT="));  Serial.println(gps.location.lat(), 6);
            Serial.print(F("LONG=")); Serial.println(gps.location.lng(), 6);
            Serial.print(F("ALT="));  Serial.println(gps.altitude.meters());
            Serial.print(F("hdop="));  Serial.println(gps.hdop.value());
            Serial.print(F("sats="));  Serial.println(gps.satellites.value());
        }
        else{
            fix = 0;
            strength = sats_fix;
            //lcd_gps_signal(lcd, strength);
        } 
        //Serial.print(F("Distance: ")); Serial.println(distance);
    }

    if(millis() > sleep_time + SLEEP_TIME_MS){
        go_sleep();
    }
}

void serial_receive(){

    char _inByte;
    String _data;
    double _lat, _lon;
    int _hash_count = 0;
    short _radius;

    while (Serial.available() > 0) {
        delay(200);
        // get incoming byte:
        _inByte = Serial.read();
        if(_inByte != '\n' || _inByte != 13 && _inByte == '#'){
            while(_inByte == '#') {
                _hash_count++;
                _inByte = Serial.read();
            }
            if (_hash_count >= 3 ) {
                delay(200);
                //Serial.println(F("Reading lattitude"));
                while (Serial.available()) {

                    if(_inByte != '#'){
                        //Serial.write(_inByte);
                        _data.concat(_inByte);
                    }
                    else {
                        //Serial.println();
                        //Serial.print(F("Got lattitude: "));
                        //Serial.println(_data);
                        break;
                    }
                    _inByte = Serial.read();
                }

                _lat = _data.toFloat();
                Serial.print(F("Received lat: ")); Serial.println(_data);
                //Serial.print(F("Received lat: ")); Serial.println(_lat, 6);

                _data = "";
                _inByte = Serial.read();
                while (Serial.available()) {

                    if(_inByte != '#'){
                        //Serial.write(_inByte);
                        _data.concat(_inByte);
                    }
                    else {
                        //Serial.println();
                        //Serial.print(F("Got longitude: "));
                        //Serial.println(_data);
                        break;
                    }
                    _inByte = Serial.read();
                }

                _lon = _data.toFloat();
                Serial.print(F("Received lon: ")); Serial.println(_data);
                //Serial.print(F("Received lon: ")); Serial.println(_lon, 6);

                _data = "";
                _inByte = Serial.read();
                while (Serial.available()) {

                    if(_inByte != '$'){
                        //Serial.write(_inByte);
                        _data.concat(_inByte);
                    }
                    else {
                        //Serial.println();
                        //Serial.print(F("Got radius: "));
                        //Serial.println(_data);
                        break;
                    }
                    _inByte = Serial.read();
                }
                _radius = _data.toInt();

                Serial.print(F("Received radius: ")); Serial.println(_radius);

                if (_inByte == '$'){
                    Serial.println(F("Received OK"));
                    single_target_lat = _lat;
                    single_target_lon = _lon;
                    radius = _radius;
                    EEPROM.put(EEPROM_INDEX_LAT, single_target_lat);
                    EEPROM.put(EEPROM_INDEX_LON, single_target_lon);
                    EEPROM.put(EEPROM_INDEX_RAD, radius);
                }
                else {
                    Serial.print(F("Unrecognised char 4: ")); Serial.println(_inByte);
                    break;
                }
            }
            else {
                Serial.println(F("Error: #'s"));
                break;
            }
        } else {
            Serial.print(F("Unrecognised char 3: ")); Serial.println(_inByte);
            break;
        }
    }
}


void go_sleep(void)
{
    lcd.clear();
    lcd.print(F("Going to sleep.."));
    delay(2000);
    pinMode(ON_PIN, OUTPUT);
    digitalWrite(ON_PIN, 0);
}


int step1()
{
    //Wait for GPS
    lcd.clear();
    lcd.println(F("Looking for GPS"));
    lcd.setCursor(0, 1);
    char str[32];
    sprintf(str,"view:%2d  fix:%2d", sats_view, sats_fix);
    lcd.print(str);

    delay(1000);

    if(millis() > sleep_time + 300000) digitalWrite(ON_PIN, 0);
    if(gps_fix == 3) return 1;
    else return 0;
}

void open_box(bool lock){
    lcd_open_box(lcd);
    delay(4000);
    lcd_box_open(lcd);
    digitalWrite(SERVO_ON_PIN, 1);
    servo.write(DOOR_OPEN);
    delay(5000);
    //servo.detach();
    if (lock) {
        digitalWrite(SERVO_ON_PIN, 0);
    }
    else {
        unsigned long ts = millis();
        while (ts + 60000 > millis()) {
            secret_button();
        }
        go_sleep();
    }


}

void close_box(){
    lcd.clear();
    lcd.print(F("  Locking box  "));
    Serial.println(F("Locking box"));
    // servo.attach(SERVO_PIN);
    digitalWrite(SERVO_ON_PIN, 1);
    delay(5000);
    servo.write(DOOR_CLOSED);


    delay(1000);




    // delay(5000);
    //servo.detach();
    digitalWrite(SERVO_ON_PIN, 0);
}


void secret_button()
{
    static unsigned long but1;
    but1 = millis();
    Serial.println(F("1"));
    while(!digitalRead(ON_PIN)){
        delay(20);
        if (millis() > but1 + 5000){
            go_sleep();
        }
    }
    but1 = millis();
    while(millis() < but1 + BUTTON1_DELAY){delay(20);
        if(!digitalRead(ON_PIN)){
            Serial.println(F("2"));
            while(!digitalRead(ON_PIN)){delay(20);}
            but1 = millis();
            while(millis() < but1 + BUTTON1_DELAY){delay(20);
                if(!digitalRead(ON_PIN)){
                    Serial.println(F("3"));
                    while(!digitalRead(ON_PIN)){delay(20);}
                    but1 = millis();
                    while(millis() < but1 + BUTTON1_DELAY){delay(20);
                        if(!digitalRead(ON_PIN)){
                            Serial.println(F("4"));
                            while(!digitalRead(ON_PIN)){delay(20);}
                            but1 = millis();
                            while(millis() < but1 + BUTTON1_DELAY){delay(20);
                                if(!digitalRead(ON_PIN)){
                                    Serial.println(F("5"));
                                    but1 = millis();
                                    while(!digitalRead(ON_PIN)){delay(20);
                                        Serial.println(F("6"));
                                        if(millis() > but1 + 800){
                                            Serial.println(F("Release now!"));

                                            lcd.clear();
                                            lcd.setCursor(0, 0);
                                            lcd.print(F("Release now!"));
                                            delay(500);

                                            lcd.clear();
                                            lcd.setCursor(0, 0);
                                            lcd.print(F("Press: open box"));
                                            lcd.setCursor(0,1);
                                            lcd.print(F("Hold: reset game"));

                                            delay(1000);

                                            while(digitalRead(ON_PIN)){}
                                            but1 = millis();
                                            while(!digitalRead(ON_PIN)){delay(20);}

                                            if(millis() - but1 > 1000){
                                                lcd.clear();
                                                lcd.print(F("RESET GAME!"));
                                                delay(3000);
                                                target = 1;
                                                EEPROM.write(EEPROM_TARGET_INDEX, 1);

                                                break;
                                            }
                                            else {
                                                open_box(1);
                                                delay(15000);
                                                close_box();
                                                break;
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
}


int distance2(double lat1, double lon1, double lat2, double lon2, unsigned long *dist)
{
    if(lat1 != 0 && lon1 != 0){
        double deg_to_rad =  3.14/180;

        double R = 6371000; // metres
        double fi1 = lat1 * deg_to_rad;
        double fi2 = lat2  * deg_to_rad;
        double delta_fi = (lat2-lat1) * deg_to_rad;
        double delta_lambda = (lon2-lon1) * deg_to_rad;

        double a = sin(delta_fi/2) * sin(delta_fi/2) +
                cos(fi1) * cos(fi2) *
                sin(delta_lambda/2) * sin(delta_lambda/2);

        d = R * ( 2 * atan2(sqrt(a), sqrt(1-a)) );
        *dist = d;
        return d;
    }
    else return -1;
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

    float bat_percent = (bat_volt - 2.75)*111.1;

    if (bat_percent > 100) bat_percent = 100;
    if (bat_percent < 0)   bat_percent = 0;
#ifdef DEBUG
    Serial.print(F("Bat voltage:")); Serial.println(bat_volt);
    Serial.print(F("Bat percent:")); Serial.println(bat_percent);
#endif
    return bat_volt;
}

