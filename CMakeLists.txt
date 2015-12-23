cmake_minimum_required(VERSION 3.3)
project(Temperatur_Gas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Arduino-Makefile/examples/BlinkInAVRC/blink.c
    Arduino-Makefile/examples/toneMelody/pitches.h
    lib/OneWire/OneWire.cpp
    lib/OneWire/OneWire.h
    lib/PID_v1/PID_v1.cpp
    lib/PID_v1/PID_v1.h
    lib/RCSwitch/RCSwitch.cpp
    lib/RCSwitch/RCSwitch.h
    lib/TimerOne/TimerOne.cpp
    lib/TimerOne/TimerOne.h
    config.h
    ds18b20.h
    Temperatur_Gas.ino)

add_executable(Temperatur_Gas ${SOURCE_FILES})