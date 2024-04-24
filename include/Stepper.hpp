#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#include <Arduino.h>

#define DEFAULT_MAX_STEPPER_FREQ 2000
#define DEFAULT_MIN_STEPPER_FREQ 100

class Stepper {
private:
    int pul_pin;
    int dir_pin;
    int min_freq;
    int max_freq;
    float lerp(float x, float a1, float b1, float a2, float b2){return a2 + (x - a1) * (b2 - a2) / (b1 - a1);}

public:

    /**
     * Constructor for Stepper motor driver
     * @param pul_pin The pin to use for the pulse signal
     * @param dir_pin The pin to use for the direction signal
     * @param min_freq The minimum frequency to use for the stepper motor. Default is 100 Hz
     * @param max_freq The maximum frequency to use for the stepper motor. Default is 2000 Hz
    */
    Stepper(int pul_pin, int dir_pin, int min_freq = DEFAULT_MIN_STEPPER_FREQ, int max_freq = DEFAULT_MAX_STEPPER_FREQ) {
        this->pul_pin = pul_pin;
        this->dir_pin = dir_pin;
        this->min_freq = min_freq;
        this->max_freq = max_freq;
        pinMode(pul_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);
    }

    /**
     * Set the speed of the stepper motor
     * @param speed The speed to set the motor to. -255 to 255
    */
    void setSpeed(int speed) {
        int dir = speed < 0 ? 1 : 0;
        speed = abs(constrain(speed, -255, 255));
        digitalWrite(dir_pin, dir);
        analogWrite(pul_pin, (speed != 0 ? 128 : 0));
        analogWriteFrequency(pul_pin, (int)lerp(speed, 0, 255, min_freq, max_freq));
    }
};

#endif // STEPPER_DRIVER_H
