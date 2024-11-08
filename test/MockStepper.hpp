#ifndef MOCK_STEPPER_H
#define MOCK_STEPPER_H

#include "Stepper.hpp"

#include <gmock/gmock.h>
#include <iostream>

class MockStepper : public Stepper {
private:
    unsigned long pulseTimer;
public:
    MockStepper(int pul_pin, 
                int dir_pin, 
                int min_freq = DEFAULT_MIN_STEPPER_FREQ, 
                int max_freq = DEFAULT_MAX_STEPPER_FREQ) : 
                Stepper(pul_pin, dir_pin, min_freq, max_freq), pulseTimer(0) {}

    MOCK_METHOD(void, initPin, (uint8_t), (override));
    MOCK_METHOD(void, stepperWriteFast, (uint8_t, uint8_t), (override));
    MOCK_METHOD(void, stepperToggleFast, (uint8_t), (override));
    MOCK_METHOD(unsigned long, getPulseTimer, (), (override));
    MOCK_METHOD(void, setPulseTimer, (unsigned long), (override));
  
    void print(const std::string& val) override {
        std::cout << val << std::endl;
    }
};

#endif // MOCK_STEPPER_H
