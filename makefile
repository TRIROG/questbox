ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ARDUINO_DIR  = arduino-1.8.3

TARGET       = QwestBox
ARDUINO_LIBS = LiquidCrystal_I2C Wire EEPROM NMEA TinyGPSPlus SoftwareSerial Servo PWMServo

BOARD_TAG    = nano
MCU = atmega328p

ARDUINO_PORT = /dev/ttyUSB0

ARDMK_DIR = Arduino-Makefile
USER_LIB_PATH = libs

#AVRDUDE_ARD_BAUDRATE = 57600

GIT_VERSION_LONG=$(shell echo $(git describe --abbrev=4 --dirty --always --tags)-$(git symbolic-ref --short HEAD)-$(date +%s))
GIT_VERSION_SHORT=$(shell echo $(git describe --abbrev=4 --dirty --always --tags))



include $(ROOT_DIR)/Arduino.mk



