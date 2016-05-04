// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef QwestBox2_H_
#define QwestBox2_H_

#include "Arduino.h"

//#include "FastSerial/FastSerial.h"
#include "HardwareSerial.h"

//#define MULTIPLE_TRAGETS
#define NUMBER_OF_TARGETS 14
#define TARGET_RESET 0



// Lutkovno gledalisce
#define TARGET_1_LAT 46.050491
#define TARGET_1_LON 14.510204


// Vhod na grad
#define TARGET_2_LAT 46.047716
#define TARGET_2_LON 14.509190

// Cerkev sveteg Florjana
#define TARGET_3_LAT 46.046268
#define TARGET_3_LON 14.508479

// Kip
#define TARGET_4_LAT 46.046011
#define TARGET_4_LON 14.5086

// Trg FR
#define TARGET_5_LAT 46.047012
#define TARGET_5_LON 14.503804

// Napoleonov kip
#define TARGET_6_LAT 46.046995
#define TARGET_6_LON 14.502988

// Zlata ladnjica
#define TARGET_7_LAT 46.047834
#define TARGET_7_LON 14.505365

// Sustrski most
#define TARGET_8_LAT 46.0484
#define TARGET_8_LON 14.505402

// Presernov trg
#define TARGET_9_LAT 46.051317
#define TARGET_9_LON 14.506129

// Rotovz
#define TARGET_10_LAT 46.05009
#define TARGET_10_LON 14.507009

// Stolnica
#define TARGET_11_LAT 46.050555
#define TARGET_11_LON 14.508158

// Zmajski most
#define TARGET_12_LAT 46.051736
#define TARGET_12_LON 14.510461

// Mesarski most (Petkovsek 1)
#define TARGET_13_LAT 46.052001
#define TARGET_13_LON 14.509192

// Petkovsek 2 (Konec)
#define TARGET_14_LAT 46.051741
#define TARGET_14_LON 14.508149


//#define DEBUG

#define LCD_BACKLIGHT_TIME 3000
#define LCD_POWER_PIN      3
#define ON_PIN             8
#define LED_PIN            13
#define BUTTON_PIN         8
#define GPS_RX_PIN         4
#define SLEEP_TIME_MS      600000
#define BUTTON1_DELAY      1000
#define DOOR_CLOSED        110
#define DOOR_OPEN          50
#define SERVO_PIN          9
#define SERVO_ON_PIN       2
#define BATTERY_VOLTAGE_PIN A1
#define BATTERY_AVAREGE_COUNT 20
#define EEPROM_TARGET_INDEX 10
#define EEPROM_INDEX_LAT 20
#define EEPROM_INDEX_LON 30
#define EEPROM_INDEX_RAD 40


//float bat_voltage;

float get_battery_voltage_avg(float bat_read);

//add your includes for the project here


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project here




//Do not add code below this line
#endif /* QwestBox2_H_ */

