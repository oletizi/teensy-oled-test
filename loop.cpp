//
// Created by Orion Letizi on 2/2/23.
//

#include "loop.h"
#include "cc_map.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=64,105
AudioMixer4              delay_input_mixer;         //xy=142,240
AudioFilterLadder        ladder1;        //xy=198,115
AudioAmplifier           delay_feedback;           //xy=298,372
AudioEffectEnvelope      amp_env;      //xy=328,108
AudioEffectDelay         delay1;         //xy=329,269
AudioEffectFreeverb      freeverb1;      //xy=337,169
AudioMixer4              mixer1;         //xy=503,121
AudioAmplifier           master_vol;           //xy=649,122
AudioOutputI2S           i2s1;           //xy=862,132
AudioAnalyzeRMS          rms1;           //xy=862,180
AudioAnalyzePeak         peak1;          //xy=862,226
AudioConnection          patchCord1(waveform1, 0, ladder1, 0);
AudioConnection          patchCord2(delay_input_mixer, delay1);
AudioConnection          patchCord3(ladder1, amp_env);
AudioConnection          patchCord4(delay_feedback, 0, delay_input_mixer, 1);
AudioConnection          patchCord5(amp_env, 0, mixer1, 0);
AudioConnection          patchCord6(amp_env, freeverb1);
AudioConnection          patchCord7(amp_env, 0, delay_input_mixer, 0);
AudioConnection          patchCord8(delay1, 0, delay_feedback, 0);
AudioConnection          patchCord9(delay1, 0, mixer1, 2);
AudioConnection          patchCord10(freeverb1, 0, mixer1, 1);
AudioConnection          patchCord11(mixer1, master_vol);
AudioConnection          patchCord12(master_vol, 0, i2s1, 0);
AudioConnection          patchCord13(master_vol, 0, i2s1, 1);
AudioConnection          patchCord14(master_vol, rms1);
AudioConnection          patchCord15(master_vol, peak1);
AudioControlSGTL5000     sgtl5000_1;     //xy=74,498
// GUItool: end automatically generated code


Adafruit_SSD1306 display(4);
const uint16_t AUDIO_MEMORY = 192;
float audio_cpu;
float audio_cpu_max;
float audio_ram;
float audio_ram_max;

short counter = 0;
short direction = -1;
float osc_freq = 440;
float rms_val = 0;
float peak_val = 0;
uint32_t t0 = 0;

__attribute__((unused)) void doSetup() {
    Serial.begin(9600);

    t0 = millis();
    // Set up MIDI
    usbMIDI.setHandleNoteOn(handleNoteOn);
    usbMIDI.setHandleNoteOff(handleNoteOff);
    usbMIDI.setHandleControlChange(handleControlChange);
    usbMIDI.begin();

    // Set up audio
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);

    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(AUDIO_MEMORY);

    // Comment these out if not using the audio adaptor board.
    // This may wait forever if the SDA & SCL pins lack
    // pullup resistors
    sgtl5000_1.enable();
    sgtl5000_1.volume(1); // caution: very loud - use oscilloscope only!
    master_vol.gain(0.5);

    // configure mixer
    mixer1.gain(0, 0.5); // dry
    mixer1.gain(1, 0.25); // reverb
    mixer1.gain(2, 0.5); // delay

    // configure reverb
    freeverb1.roomsize(0.5);

    // configure delay
    delay1.delay(0, 250);
    for (int i = 1; i < 8; i++) {
        // turn off the taps we're not using
        delay1.disable(i);
    }
    delay_feedback.gain(0.5);
    delay_input_mixer.gain(0, 0.5); // dry input
    delay_input_mixer.gain(1, 0.5); // feedback input

    // initialize amp envelope
    amp_env.attack(50);
    amp_env.hold(0);
    amp_env.decay(0);
    amp_env.sustain(1);
    amp_env.delay(0);

    amp_env.release(50);

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
    uint32_t now = millis();
    if (now - t0 >= 500) {
        t0 = now;
    }

    usbMIDI.read();
    AudioNoInterrupts();
    uint16_t cpu_filt = ladder1.cpu_cycles;
    uint16_t cpu_max_filt = ladder1.cpu_cycles_max;
    uint16_t cpu_osc = waveform1.cpu_cycles;
    rms_val = rms1.read();
    if (t0 == now) {
        peak_val = peak1.read();
    }
    audio_cpu = AudioProcessorUsage();
    audio_ram = AudioMemoryUsage();
    AudioInterrupts();
    //digitalWrite(LED_BUILTIN, HIGH);
    display.clearDisplay();
    //display.setTextColor(WHITE);
    //display.setCursor(0, 24);
    //display.printf("Count: %d\n", counter);
    //display.printf("cpu: %f\n", audio_cpu);
    //display.printf("mem: %f\n", audio_ram);
    //display.printf("Peak : %f\n", peak_val);
    //display.drawCircle(counter, 16, 5, WHITE);
    display.setTextColor(WHITE);

    uint8_t label_width = 24;

    float rms_indicator_value = scale(rms_val, 0, 1, 0, 127 - label_width, 1);
    float peak_indicator_value = scale(peak_val, 0, 1, 0, 127 - label_width, 1);

    // Draw RMS meter
    display.setCursor(0, 0);
    display.print("RMS");
    display.fillRect(label_width + 1, 0, rms_indicator_value, 7, WHITE);
    display.drawFastVLine(label_width + 1 + peak_indicator_value, 0, 7, WHITE);

    // Draw memory meter
    float ram_indicator_value = scale(audio_ram, 0, AUDIO_MEMORY, 0, 127 - label_width, 1);
    display.fillRect(label_width + 1, 8, ram_indicator_value, 7, WHITE);
    display.setCursor(0, 8);
    display.println("RAM");

    // Draw CPU meter
    float cpu_indicator_value = scale(audio_cpu, 0, 100, 0, 127 - label_width, 1);
    display.fillRect(label_width + 1, 16, cpu_indicator_value, 7, WHITE);
    display.setCursor(0, 16);
    display.println("CPU");

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
    amp_env.noteOn();
    AudioInterrupts();
    Serial.printf("Set frequency to: %f\n", osc_freq);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    AudioNoInterrupts();
    amp_env.noteOff();
    AudioInterrupts();
}

void handleControlChange(byte channel, byte control, byte value) {
    Serial.printf("CC: chan: %d, control: %d, val: %d", channel, control, value);
    digitalWrite(LED_BUILTIN, HIGH);
    float feedback_value = 0;
    AudioNoInterrupts();
    switch (control) {
        case CC_VOLUME:
            master_vol.gain(scale(value, 0, 127, 0, 1, 1.1));
            break;
        case CC_LPF_CUTOFF:
            ladder1.frequency(scale(value, 0, 127, 0, 20000, 1.02));
            break;
        case CC_LPF_RESONANCE:
            ladder1.resonance(scale(value, 0, 127, 0, 1, 1));
            break;
        case CC_REVERB_TIME:
            freeverb1.roomsize(scale(value, 0, 127, 0, 1, 1));
            break;
        case CC_REVERB_CUTOFF:
            freeverb1.damping(scale(value, 0, 127, 0, 1, 1));
            break;
        case CC_REVERB_BALANCE:
            mixer1.gain(1, scale(value, 0, 127, 0, 1, 1));
            break;
        case CC_DELAY_TIME:
            delay1.delay(0, scale(value, 0, 127, 0, 500, 1));
            break;
        case CC_DELAY_FEEDBACK:
            feedback_value = scale(value, 0, 127, 0, 1, 1.02);
            delay_feedback.gain(feedback_value);
            break;
        case CC_DELAY_BALANCE:
            mixer1.gain(2, scale(value, 0, 127, 0, 1, 1));
            break;
        default:
            break;

    }
    AudioInterrupts();
    Serial.printf("Delay feebdack: %f\n", feedback_value);
}

float m2f(int note) {
    float a = 440; //frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((note - 9) / 12.0));
}

float scale(float in, float inlow, float inhigh, float outlow, float outhigh, float power) {
    float value;
    float inscale = safediv(1., inhigh - inlow);
    float outdiff = outhigh - outlow;

    value = (in - inlow) * inscale;
    if (value > 0.0)
        value = pow(value, power);
    else if (value < 0.0)
        value = -pow(-value, power);
    value = (value * outdiff) + outlow;

    return value;
}