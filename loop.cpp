//
// Created by Orion Letizi on 2/2/23.
//

#include "loop.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform waveform1;      //xy=119,156
AudioFilterLadder ladder1;        //xy=281,153
AudioOutputI2S i2s1;           //xy=420,155
AudioConnection patchCord1(waveform1, 0, ladder1, 0);
AudioConnection patchCord2(ladder1, 0, i2s1, 0);
AudioConnection patchCord3(ladder1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;     //xy=299,289
// GUItool: end automatically generated code






Adafruit_SSD1306 display(4);

int counter = 0;
int direction = -1;

__attribute__((unused)) void doSetup() {
    Serial.begin(9600);

    // Set up audio
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);

    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(24);

    // Comment these out if not using the audio adaptor board.
    // This may wait forever if the SDA & SCL pins lack
    // pullup resistors
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.5); // caution: very loud - use oscilloscope only!

    // initialize filter
    ladder1.frequency(2000);
    ladder1.resonance(0.7);
    ladder1.inputDrive(1);

    // initialize oscillator
    waveform1.frequency(440);
    waveform1.amplitude(1.0);
    waveform1.begin(WAVEFORM_SAWTOOTH);

    // Set up LED
    pinMode(LED_BUILTIN, OUTPUT);

    // Set up display
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
}


__attribute__((unused)) void doLoop() {

    AudioNoInterrupts();
    uint16_t cpu_filt = ladder1.cpu_cycles;
    uint16_t cpu_max_filt = ladder1.cpu_cycles_max;
    uint16_t cpu_osc = waveform1.cpu_cycles;
    float osc_freq = 220.0f + (float(counter) * 2.0f);
    float filt_freq = osc_freq * 8;

    waveform1.frequency(osc_freq);
    ladder1.frequency(filt_freq);
    AudioInterrupts();

    digitalWrite(LED_BUILTIN, HIGH);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 8);
    //display.printf("Count: %d\n", counter);
    display.printf("Of: %d, Ff: %d\n", int(osc_freq), int(filt_freq));
    display.printf("F cpu: %d, m: %d\n", cpu_filt, cpu_max_filt);
    display.printf("O cpu : %d\n", cpu_osc);
    //display.drawCircle(counter, 16, 5, WHITE);
    display.drawFastVLine(counter - 1, 0, 7, WHITE);
    display.display();

    digitalWrite(LED_BUILTIN, LOW);
    //delay(DELAY);
    if (counter % 128 == 0) {
        direction *= -1;
    }
    counter = counter + direction;
}
