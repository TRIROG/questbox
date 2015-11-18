// Do not remove the include below
#include "QwestBox.h"



//#include <Wire.h>
#include "LiquidCrystal_I2C.h"
//#include <avr/sleep.h>
//#include <MsTimer2.h>
//#include <TimerOne.h>
//#include <EEPROM.h>
//#include <nmea.h>
#include "TinyGPS++.h"
//#include <TinyGPS++.h>


#include <SoftwareSerial.h>
//#include <LiquidCrystal.h>
//#include <TinyGPS.h>
#include <Servo.h>
#include <Servo/src/Servo.h>

//#include <DigitalToggle.h>
//#include </home/vid/Programs/arduino-1.6.4/libraries/DigitalToggle/DigitalToggle.h>
//#include

#define LCD_BACKLIGHT_TIME 3000
#define LCD_POWER_PIN      3
#define ON_PIN             8
#define BUTTON_PIN         2
#define SLEEP_TIME_MS      600000
#define BUTTON1_DELAY      1000
#define DOOR_CLOSED        110
#define DOOR_OPEN          50
#define SERVO_PIN          9

#define _useTimer1

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

//NMEA gps(ALL);
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

unsigned long lcd_time;
unsigned long time = 0;
unsigned long sleep_time = 0;
double lat, lon;
double lat2, lon2;
double d, distance;
int sats_view, sats_fix, gps_fix;
long hdop;
bool newData = false;
unsigned long chars;
unsigned short sentences, failed;
int but;
int test = 0;
bool interrupt_flag = 0;



void setup()
{
//46.080686, 14.550043

  lat2 = 46.080686;
  lon2 = 14.550043;


  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  //pinMode(4, INPUT);
  //pinMode(5, OUTPUT);
  pinMode(ON_PIN, OUTPUT);
  pinMode(LCD_POWER_PIN, OUTPUT);
  servo.attach(SERVO_PIN);

  digitalWrite(ON_PIN, 1);
  digitalWrite(LCD_POWER_PIN, 1);
 // MsTimer2::set(1000, go_test);
  //Serial.println(MsTimer2::count);
  //MsTimer2::start();
  //Serial.println(MsTimer2::count);
  //interrupts();
  //Timer1.initialize(100000);
  //Timer1.attachInterrupt(go_sleep);

  Serial.begin(115200);

      // delay(10000);
  Serial.println("QuestBox");
  Serial.println();

  lcd.init();
  lcd.backlight();
  lcd.print("   Quest Box  ");
  //lcd.begin(16, 2);

  servo.write(90);
  delay(200);
  servo.write(DOOR_CLOSED);
  delay(1500);
  servo.detach();
  ss.begin(9600);

  Serial.println("Setup done.");
}

void loop()
{
  //Serial.println("Loop started.");

  but= digitalRead(BUTTON_PIN);
  if(!but){
    secret_button();
  }

//
//
//
//    digitalWrite(13, 1);
//
//    //servo.write(DOOR_OPEN);
//    lcd.init();
//    lcd.backlight();
//    lcd.clear();
//    lcd.print("   Quest Box  ");
//    lcd_time = millis();
//    sleep_time = millis();
//    delay(3000);
//
//
//
//    while(but){but = digitalRead(BUTTON_PIN); delay(30);}


//  else{
//    //digitalWrite(13, 0);
//    //lcd.noBacklight();
//    servo.write(DOOR_CLOSED);
//  }

  //if(ss.available())
   // get_gps();

   while (ss.available() > 0)
      gps.encode(ss.read());

      sats_fix = gps.satellites.value();
      hdop = gps.hdop.value();
      distance = gps.distanceBetween(lat, lon, lat2, lon2);
      lat = gps.location.lat();
      lon = gps.location.lng();



  //Serial.println(time);
  if(millis() > time + 2000){
//    Serial.println(millis());
    digitalToggle(13);

      Serial.println(gps.location.lat(), 6);

      Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
      Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
      Serial.print("ALT=");  Serial.println(gps.altitude.meters());
      Serial.print("hdop=");  Serial.println(gps.hdop.value());
      Serial.print("sats=");  Serial.println(gps.satellites.value());





//    Serial.print("Sats in view = "); Serial.println(sats_view);
//    Serial.print("Sats fix = "); Serial.println(sats_fix);
//    Serial.print("GPS fix = "); Serial.println(gps_fix);
//
//    Serial.print("lon = "); Serial.println(lon);
//    Serial.print("lat = "); Serial.println(lat);
//
//    Serial.print("distance is "); Serial.print(d); Serial.println(" m");


    if(hdop < 500 && sats_fix > 3){
      distance2();
      lcd.clear();
      lcd.print("Your goal is");
      lcd.setCursor(0,1);
      lcd.print(distance);
      lcd.print("m away...");
      delay(8000);
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
    if(distance){
      lcd.clear();
      lcd.print("Going to sleep");
      delay(3000);
      go_sleep();
    }


    Serial.println("Saving time");
    Serial.println(time);
    time = millis();
  }

  if(millis() > sleep_time + SLEEP_TIME_MS){
    lcd.clear();
    lcd.print("    No GPS");
    lcd.setCursor(0, 1);
    lcd.print("Going to sleep..");
    delay(5000);

    go_sleep();
  }


  //delay(100);

}

//
void digitalToggle(int P)
{
	if(digitalRead(P) == 1) digitalWrite(P, 0);
	else digitalWrite(P, 1);
}

void go_sleep(void)
{

  digitalWrite(ON_PIN, 0);
//
//  //MsTimer2::stop();
//  //noInterrupts();
//
//  Serial.println("Going to sleep.");
//  digitalWrite(LCD_POWER_PIN, 0);
//
//  for(int i = 0; i < 20; i++){
//    digitalToggle(13);
//    delay(50);
//  }
//
//  digitalWrite(13, 0);
//  attachInterrupt(0, pin2_isr, CHANGE);
//  /* 0, 1, or many lines of code here */
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//  cli();
//  sleep_enable();
//  sleep_bod_disable();
//  sei();
//  sleep_cpu();
//  /* wake up here */
//  Serial.println("Waking up.");
//
//  sleep_time = millis();
//  interrupt_flag = 1;
//  sleep_disable();
}

void pin2_isr()
{
  //flagdetachInterrupt(0);
  interrupt_flag = 1;
}

void get_gps()
{
//  Serial.println("Reading GPS");
//  while (ss.available()) {
//
//    //Serial.println(ss.read());
//    if (gps.encode(ss.read())) {
//        char *c = gps.term(0);
//
//        //Serial.println(c);
//
//        if(strcmp(c, "GPGGA") == 0){
//          gps_fix = atoi(gps.term(4));
//          sats_fix = atoi(gps.term(7));
//          //Serial.println(gps.term(1));
//        }
//
//        if(strcmp(c, "GPGSV") == 0){
//          sats_view = atoi(gps.term(3));
//        }
//
//        lon = gps.gprmc_longitude();
//        lat = gps.gprmc_latitude();
//      }
//    }
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
  if(lat != 0 && lon != 0){
    double deg_to_rad =  3.14/180;

    double R = 6371000; // metres
    double fi1 = lat * deg_to_rad;
    double fi2 = lat2  * deg_to_rad;
    double delta_fi = (lat2-lat) * deg_to_rad;
    double delta_lambda = (lon2-lon) * deg_to_rad;

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
  servo.attach(9);
  servo.write(DOOR_OPEN);
  delay(10000);
  servo.write(DOOR_CLOSED);
  delay(2000);
  servo.detach();
}


void secret_button()
{
    static unsigned long but1, but2;
    Serial.println("1");
    while(digitalRead(BUTTON_PIN)){delay(20);}
      but1 = millis();
      while(millis() < but1 + BUTTON1_DELAY){delay(20);
        if(!digitalRead(BUTTON_PIN)){
          Serial.println("2");
          while(!digitalRead(BUTTON_PIN)){delay(20);}
          but1 = millis();
          while(millis() < but1 + BUTTON1_DELAY){delay(20);
            if(!digitalRead(BUTTON_PIN)){
              Serial.println("3");
              while(!digitalRead(BUTTON_PIN)){delay(20);}
               but1 = millis();
               while(millis() < but1 + BUTTON1_DELAY){delay(20);
               if(!digitalRead(BUTTON_PIN)){
                 Serial.println("4");
                 while(!digitalRead(BUTTON_PIN)){delay(20);}
                 but = millis();
                 while(millis() < but1 + BUTTON1_DELAY){delay(20);
                 if(!digitalRead(BUTTON_PIN)){
                   Serial.println("5");
                   but1 = millis();
                   while(!digitalRead(BUTTON_PIN)){delay(20);
                     Serial.println("6");
                     if(millis() > but1 + 2000){
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


