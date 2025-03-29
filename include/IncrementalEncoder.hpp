#pragma once

#include <Arduino.h>

// could also look into dedicated libraries or built in hardware encoder support
// currently each 'tick' results in multiple calls to the interrupt handler despite the encoder only moving once
// need to look into why this is happening

// TODO debounce the signal (prevent redundant interrupt calls which slow down other processes). this may or may not be the culprit
// TODO hardware encoder support (if possible)

// https://forum.pjrc.com/index.php?threads/using-5v-rotary-encoders-with-teensy-4-1.72834/

static const int8_t ENC_STATES[] = { 0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0 };

class IncrementalEncoder
{
private:
    static IncrementalEncoder *encoders[16];
    static int encoderCount;

    long angle;
    uint8_t prev;
    int pin_a;
    int pin_b;
    int ticksPerRevolution;

public:
    IncrementalEncoder(int pin_a, int pin_b, int ticksPerRevolution = 360) {
        this->angle = 0;
        this->prev = 0;
        this->pin_a = pin_a;
        this->pin_b = pin_b;
        this->ticksPerRevolution = ticksPerRevolution;

        pinMode(pin_a, INPUT_PULLUP);
        pinMode(pin_b, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin_a), handleInterupt, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pin_b), handleInterupt, CHANGE);

        encoders[encoderCount++] = this;
    }

    static void handleInterupt() {
        for (int i = 0; i <encoderCount; i++) {
            if (encoders[i]->hasChanged()) {
                encoders[i]->update();
            }
        }
    }
    
    void update() {
        // 0b0000                       0b0000
        //   ^^ = prev state a and b        ^^ = current state a and b
        // combines into a 4 bit number that corresponds to the state of the encoder
        // for more info see https://en.wikipedia.org/wiki/Incremental_encoder

        prev <<= 2;
        prev = prev | digitalRead(pin_a) << 1 | digitalRead(pin_b);
        angle += (long)ENC_STATES[(prev & 0b1111)];
    }

    bool hasChanged() {
        return (prev & 0b11) != ((digitalRead(pin_a) << 3 | digitalRead(pin_b) << 2) & 0b11);
    }

    long getTicks() {
        return angle;
    }

    double getAngleDegrees() {
        return (angle * 360.0) / ticksPerRevolution;
    }

    double getAngleRadians() {
        return (angle * 2 * PI) / ticksPerRevolution;
    }

    double getAngleRevolutions() {
        return angle / ticksPerRevolution;
    }

    void reset() {
        angle = 0L;
    }

    int getPinA() {
        return pin_a;
    }

    int getPinB() {
        return pin_b;
    }
};

IncrementalEncoder *IncrementalEncoder::encoders[16];
int IncrementalEncoder::encoderCount = 0;
