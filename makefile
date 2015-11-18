ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ARDUINO_DIR  = /home/vid/Programs/arduino-1.6.5

TARGET       = QwestBox
ARDUINO_LIBS = LiquidCrystal_I2C Wire EEPROM NMEA TinyGPSPlus SoftwareSerial Servo

BOARD_TAG    = uno
ARDUINO_PORT = /dev/ttyUSB0


include $(ROOT_DIR)/Arduino.mk



