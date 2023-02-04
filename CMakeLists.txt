project(teensy-oled-test)
cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 11)
# 1.57
set (TEENSY_ROOT "$ENV{HOME}/Library/Arduino15/packages/teensy/hardware/avr/1.57.2")
set (TEENSY_LIBS "${TEENSY_ROOT}/libraries")
set (ARDUINO_LIBS "$ENV{HOME}/Documents/Arduino/libraries")
add_executable(teensy-oled-test loop.h loop.cpp)
include_directories(
        ${TEENSY_ROOT}/cores/teensy4
        ${ARDUINO_LIBS}/Adafruit_GFX_Library
        ${ARDUINO_LIBS}/Adafruit_SSD1306
)
