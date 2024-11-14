#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#include <Arduino.h>

#include "Stepper.hpp"

class StepperDriver : public Stepper {
private:
    elapsedMicros pulseTimer;
public:
    void initPin(uint8_t pin) override {
        pinMode(pin, OUTPUT_OPENDRAIN);
    }

    void stepperWriteFast(uint8_t pin, uint8_t val) override {
        digitalWriteFast(pin, val);
    }

    void stepperToggleFast(uint8_t pin) override {
        digitalToggleFast(pin);
    }

    unsigned long getPulseTimer() override {
        return pulseTimer;
    }

    void setPulseTimer(unsigned long val) override {
        pulseTimer = val;
    }

    void print(const std::string& val) override {
        Serial.println(val);
    }
};

#endif // STEPPER_DRIVER_H
