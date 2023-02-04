//
// Created by Orion Letizi on 2/2/23.
//
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "loop.h"

Adafruit_SSD1306 display(4);
void doSetup() {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
}

unsigned long now = 0;
unsigned long checkpoint = 0;

void doLoop() {
    // nothing happens after setup
    now = millis();
    if (now - checkpoint >= 1000) {
        checkpoint = now;
        Serial.printf("At the tone, the time will be: %lu\n", now);
    }
}
