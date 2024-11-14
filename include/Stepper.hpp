#ifndef STEPPER_H
#define STEPPER_H

#define DEFAULT_MAX_STEPPER_FREQ 1000
#define DEFAULT_MIN_STEPPER_FREQ 100

class Stepper {
private:
    int pul_pin;
    int dir_pin;
    int min_freq_hz;
    int max_freq_hz;
    int freq_hz = 0;
    unsigned long period_us = 0;
    float lerp(float x, float a1, float b1, float a2, float b2){return a2 + (x - a1) * (b2 - a2) / (b1 - a1);}

    // defined here to avoid using Arduino.h
    template <typename T>
    static T constrain(T val, T lower, T upper) {
        if(val > upper) return upper;
        if(val < lower) return lower;
        return val;
    }

public:
    /**
     * @brief Set the given pin to the given mode
     * 
     * @param pin the pin to set mode of
     * @param mode the mode (usually given via enumeration)
     */
    virtual void initPin(uint8_t pin) = 0;

    /**
     * @brief Write a value to the given pin
     * 
     * @param pin The pin to write to
     * @param val The value to write
     */
    virtual void stepperWriteFast(uint8_t pin, uint8_t val) = 0;

    /**
     * @brief Toggle the given pin
     * 
     * @param pin The pin to toggle
     */
    virtual void stepperToggleFast(uint8_t pin) = 0;

    /**
     * @brief Get the current pulse timer time
     * 
     * @return the current pulse timer time
     */
    virtual unsigned long getPulseTimer() = 0;

    /**
     * @brief Set the pulse timer value
     * 
     * @param val the value to set the pulse timer to
     */
    virtual void setPulseTimer(unsigned long val) = 0;

    virtual void print(const std::string& val) = 0;

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
    }

    void init() {
        initPin(pul_pin);
        initPin(dir_pin);
    }

    /**
     * Set the speed of the stepper motor
     * @param speed The speed to set the motor to. -255 to 255
    */
    void setSpeed(int speed) {
        int dir = speed < 0 ? 1 : 0;
        speed = abs(constrain(speed, -255, 255));
        stepperWriteFast(dir_pin, dir);
        if (speed == 0) {
            stepperWriteFast(pul_pin, 0);
            freq_hz = 0;
            period_us = 0;
            return;
        }

        freq_hz = (int) lerp(speed, 0, 255, min_freq_hz, max_freq_hz);
        period_us = 100000 / freq_hz;

        print("Speed: " + std::to_string(speed) + " Freq: " + std::to_string(freq_hz) + " Period: " + std::to_string(period_us));
        
    }

    /**
     * Update the pulse signal for the stepper motor
     * must be called at sufficient frequency. very minimum of 3-4 times the period of the signal per second
    */
    void updatePin() {
        if (freq_hz == 0) {
            return;
        }

        if (getPulseTimer() > period_us) {
            setPulseTimer(0);
            stepperToggleFast(pul_pin);
        }
    }
};

#endif // STEPPER_H
