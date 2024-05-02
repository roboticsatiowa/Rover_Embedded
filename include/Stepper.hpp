#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#include <Arduino.h>

#define DEFAULT_MAX_STEPPER_FREQ 2000
#define DEFAULT_MIN_STEPPER_FREQ 100

class Stepper {
private:
    int pul_pin;
    int dir_pin;
    int min_freq_hz;
    int max_freq_hz;
    int freq_hz;
    unsigned long period_us;
    float lerp(float x, float a1, float b1, float a2, float b2){return a2 + (x - a1) * (b2 - a2) / (b1 - a1);}
    elapsedMicros pulseTimer;

public:

    /**
     * Constructor for Stepper motor driver
     * @param pul_pin The pin to use for the pulse signal
     * @param dir_pin The pin to use for the direction signal
     * @param min_freq The minimum frequency to use for the stepper motor. Default is 100 Hz
     * @param max_freq The maximum frequency to use for the stepper motor. Default is 2000 Hz
    */
    Stepper(int pul_pin, int dir_pin, int min_freq = DEFAULT_MIN_STEPPER_FREQ, int max_freq = DEFAULT_MAX_STEPPER_FREQ) {

        //https://forum.pjrc.com/index.php?threads/using-the-open-drain-capabilities-of-the-teensy-3-1-processor.25393/

        this->pul_pin = pul_pin;
        this->dir_pin = dir_pin;
        this->min_freq_hz = min_freq;
        this->max_freq_hz = max_freq;
        
        pinMode(pul_pin, OUTPUT_OPENDRAIN);
        pinMode(dir_pin, OUTPUT_OPENDRAIN);
    }

    /**
     * Set the speed of the stepper motor
     * @param speed The speed to set the motor to. -255 to 255
    */
    void setSpeed(int speed) {
        int dir = speed < 0 ? 1 : 0;
        speed = abs(constrain(speed, -255, 255));
        digitalWriteFast(dir_pin, dir);
        if (speed == 0) {
            digitalWriteFast(pul_pin, LOW);
            freq_hz = 0;
            period_us = 0;
            return;
        }

        freq_hz = (int) lerp(speed, 0, 255, min_freq_hz, max_freq_hz);
        period_us = 1000000 / freq_hz;
        
    }

    void updatePin() {
        if (freq_hz == 0) {
            return;
        }

        if (elapsedMicros() > period_us) {
            pulseTimer = 0;
            digitalToggleFast(pul_pin);
        }
    }
};

#endif // STEPPER_DRIVER_H
