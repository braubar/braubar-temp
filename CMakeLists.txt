cmake_minimum_required(VERSION 3.3)
project(braubar_temp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lib/OneWire/OneWire.cpp
    lib/OneWire/OneWire.h
    lib/TimerOne/TimerOne.cpp
    lib/TimerOne/TimerOne.h
    lib/Ethernet2/src/Ethernet2.h
    CMakeLists.txt
    config.h
    ds18b20.h
    Makefile
    Temperatur_Gas.ino)

add_executable(braubar_temp ${SOURCE_FILES})