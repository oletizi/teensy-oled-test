//
// Created by Orion Letizi on 2/2/23.
//

#include "loop.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MIDI.h>

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

short counter = 0;
short direction = -1;
float osc_freq = 440;
__attribute__((unused)) void doSetup() {
    Serial.begin(9600);
    // Set up MIDI
    usbMIDI.setHandleNoteOn(handleNoteOn);
    usbMIDI.setHandleNoteOff(handleNoteOff);
    usbMIDI.begin();

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
    usbMIDI.read();
    AudioNoInterrupts();
    uint16_t cpu_filt = ladder1.cpu_cycles;
    uint16_t cpu_max_filt = ladder1.cpu_cycles_max;
    uint16_t cpu_osc = waveform1.cpu_cycles;
    AudioInterrupts();
    //digitalWrite(LED_BUILTIN, HIGH);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 8);
    //display.printf("Count: %d\n", counter);
    display.printf("Ofreq: %f\n", osc_freq);
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

void handleNoteOn(byte channel, byte note, byte velocity) {
    Serial.printf("MIDI: channel: %d, note: %d, velocity: %d\n", channel, note, velocity);
    digitalWrite(LED_BUILTIN, HIGH);
    AudioNoInterrupts();
    osc_freq = m2f(note);
    waveform1.frequency(osc_freq);
    AudioInterrupts();
    Serial.printf("Set frequency to: %f\n", osc_freq);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    // nop
}

void handleControlChange(byte channel, byte control, byte value) {
    // nop
}

float m2f(int note) {
    float a = 440; //frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((note - 9) / 12.0));
}