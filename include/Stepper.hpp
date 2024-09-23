#pragma once
#include <Arduino.h>
#include "PITTimers.hpp"

#define DEFAULT_MAX_STEPPER_FREQ 1000
#define DEFAULT_MIN_STEPPER_FREQ 100

class Stepper
{
private:
	int pul_pin;
	int dir_pin;
	int min_freq_hz;
	int max_freq_hz;
	int timer_index;

	float lerp(float x,
			   float a1,
			   float b1,
			   float a2,
			   float b2)
	{
		return a2 + (x - a1) * (b2 - a2) / (b1 - a1);
	}

public:
	/**
	 * Constructor for Stepper motor driver
	 * @param pul_pin The pin to use for the pulse signal
	 * @param dir_pin The pin to use for the direction signal
	 * @param min_freq The minimum frequency to use for the stepper motor. Default is 100 Hz
	 * @param max_freq The maximum frequency to use for the stepper motor. Default is 2000 Hz
	*/
	Stepper(int pul_pin,
			int dir_pin,
			int min_freq = DEFAULT_MIN_STEPPER_FREQ,
			int max_freq = DEFAULT_MAX_STEPPER_FREQ)
	{
		this->pul_pin = pul_pin;
		this->dir_pin = dir_pin;
		this->min_freq_hz = min_freq;
		this->max_freq_hz = max_freq;
		this->timer_index = -1;

		for (int i = 0; i < NUM_TIMERS; ++i)
		{
			if (timerCallbacks[i] == nullptr)
			{
				attachTimerCallback(i, [this]() { this->updatePin(); });
				this->timer_index = i;
				break;
			}
		}

		if (this->timer_index == -1)
		{
			Serial.print("More than ");
			Serial.print(NUM_TIMERS);
			Serial.println(" PIT steppers, the newest one will not be active");
		}

		pinMode(pul_pin, OUTPUT_OPENDRAIN);
		pinMode(dir_pin, OUTPUT_OPENDRAIN);
	}

	/**
	 * Set the speed of the stepper motor
	 * @param speed The speed to set the motor to. -255 to 255
	*/
	void setSpeed(int speed)
	{
		int dir = speed < 0 ? 1 : 0;
		speed = abs(constrain(speed, -255, 255));
		digitalWriteFast(this->dir_pin, dir);

		if (speed == 0)
		{
			digitalWriteFast(this->pul_pin, LOW);
			setTimerFrequency(this->timer_index, 0);
			return;
		}

		int freq_hz = (int) lerp(speed, 0, 255, this->min_freq_hz, this->max_freq_hz);

		setTimerFrequency(this->timer_index, freq_hz);
		int period_us = 1000000 / freq_hz;

		Serial.println("Speed: " + String(speed) + " Freq: " + String(freq_hz) + " Period: " + String(period_us));
	}

	/**
	 * Update the pulse signal for the stepper motor
	 * called by an interrupt timer at the stepper frequency
	*/
	void updatePin()
	{
		digitalToggleFast(this->pul_pin);
	}
};