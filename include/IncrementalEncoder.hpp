#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

#include <Arduino.h>

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
            encoders[i]->update();
        }
    }

    void update() {
        prev <<= 2;
        prev = prev | digitalRead(pin_a) << 1 | digitalRead(pin_b);
        angle += (long)ENC_STATES[(prev & 0b1111)];
    }

    bool hasChanged() {
        return (prev & 0b11) != 0;
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


#endif