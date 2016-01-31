ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ARDUINO_DIR  = /home/vid/Programs/arduino-1.6.5

TARGET       = QwestBox
ARDUINO_LIBS = LiquidCrystal_I2C Wire EEPROM NMEA TinyGPSPlus SoftwareSerial Servo

BOARD_TAG    = nano
MCU = atmega328p

ARDUINO_PORT = /dev/ttyUSB0

ARDMK_DIR = /home/vid/Arduino/Arduino-Makefile-master



include $(ROOT_DIR)/Arduino.mk



