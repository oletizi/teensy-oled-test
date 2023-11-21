//
// Created by Orion Letizi on 2/2/23.
//

#include "loop.h"

Adafruit_SSD1306 display(4);

__attribute__((unused)) void doSetup() {
    pinMode(LED_BUILTIN, OUTPUT);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
}

unsigned long now = 0;
unsigned long checkpoint = 0;
int counter = 0;
int direction = -1;
const int DELAY = 0;
__attribute__((unused)) void doLoop() {
    digitalWrite(LED_BUILTIN, HIGH);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    //delay(DELAY);
    display.printf("Count: %d\n", counter);
    display.drawCircle(counter, 16, 5, WHITE);
    display.drawRoundRect(128 - counter, 16 -5, 10, 10, 2, WHITE);
    display.display();

    digitalWrite(LED_BUILTIN, LOW);
    //delay(DELAY);
    if (counter % 128 == 0) {
        direction *= -1;
    }
    counter = counter + direction;
}
