//
// Created by Orion Letizi on 2/2/23.
//

#ifndef ARDUINO_CLI_TEST_FOO_H
#define ARDUINO_CLI_TEST_FOO_H
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

__attribute__((unused)) void doSetup();

__attribute__((unused)) void doLoop();

void handleNoteOn(byte channel, byte note, byte velocity);
void handleNoteOff(byte channel, byte note, byte velocity);
void handleControlChange(byte channel, byte control, byte value);
float scale(float in, float inlow, float inhigh, float outlow, float outhigh, float power);
inline float safediv(float num, float denom) {
    return denom == 0. ? (float) 0. : (float) (num / denom);
}
float m2f(int note);

#endif //ARDUINO_CLI_TEST_FOO_H

