// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef QwestBox2_H_
#define QwestBox2_H_

#include "Arduino.h"

#define NUMBER_OF_TARGETS 7
#define TARGET_RESET 0

#define TARGET_1_LAT 46.047716
#define TARGET_1_LON 14.509190

#define TARGET_2_LAT 46.046268
#define TARGET_2_LON 14.508479

#define TARGET_3_LAT 46.047012
#define TARGET_3_LON 14.503804

#define TARGET_4_LAT 46.047815
#define TARGET_4_LON 14.505000

#define TARGET_5_LAT 46.050886
#define TARGET_5_LON 14.506384

#define TARGET_6_LAT 46.050595
#define TARGET_6_LON 14.508322

#define TARGET_7_LAT 46.052025
#define TARGET_7_LON 14.508843


#define LCD_BACKLIGHT_TIME 3000
#define LCD_POWER_PIN      3
#define ON_PIN             8
#define LED_PIN            13
#define BUTTON_PIN         2
#define GPS_RX_PIN         4
#define SLEEP_TIME_MS      600000
#define BUTTON1_DELAY      1000
#define DOOR_CLOSED        110
#define DOOR_OPEN          50
#define SERVO_PIN          9
#define SERVO_ON_PIN       6
#define BATTERY_VOLTAGE_PIN A1
#define BATTERY_AVAREGE_COUNT 20
#define EEPROM_TARGET_INDEX 10


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

