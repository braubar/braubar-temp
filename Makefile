PROJECT_DIR       = .
ARDMK_DIR         = $(PROJECT_DIR)/Arduino-Makefile
ARDUINO_DIR       = /usr/share/arduino
USER_LIB_PATH    :=  $(PROJECT_DIR)/lib
BOARD_TAG         = uno
MONITOR_BAUDRATE  = 9600
AVR_TOOLS_DIR     = /usr
AVRDUDE          = /usr/bin/avrdude
CFLAGS_STD        = -std=gnu11
CXXFLAGS_STD      = -std=gnu++11
CXXFLAGS         += -pedantic -Wall -Wextra
MONITOR_PORT      = /dev/ttyACM*
CURRENT_DIR       = $(shell basename $(CURDIR))
OBJDIR            = $(PROJECT_DIR)/bin/$(BOARD_TAG)/$(CURRENT_DIR)
include $(ARDMK_DIR)/Arduino.mk

