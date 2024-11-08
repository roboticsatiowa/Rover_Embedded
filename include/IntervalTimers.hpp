#pragma once

#include <Arduino.h>
#include <IntervalTimer.h>
#include <functional>

// There are only 4 timers allowed, as described in the interval timer code
const int NUM_TIMERS = 4;
IntervalTimer timers[NUM_TIMERS];

// Function pointer array which will be used for timer callbacks
using TimerCallback = std::function<void()>;
TimerCallback timerCallbacks[NUM_TIMERS] = {nullptr};

/**
 * @brief Attaches a callback function to a specific timer.
 *
 * This function associates a callback function with one of the available timers.
 * The callback will be executed when the timer triggers.
 *
 * @param timerIndex The index of the timer to attach the callback to (0 to NUM_TIMERS-1).
 * @param callback The function to be called when the timer triggers.
 */
void attachTimerCallback(int timerIndex, TimerCallback callback) {
  if (timerIndex >= 0 && timerIndex < NUM_TIMERS) {
	timerCallbacks[timerIndex] = callback;
  }
}

/**
 * @brief Sets the frequency for a specific timer.
 *
 * This function sets the frequency at which a timer will trigger its callback.
 * If the frequency is set to 0, the timer will be stopped.
 *
 * @param timerIndex The index of the timer to set the frequency for (0 to NUM_TIMERS-1).
 * @param frequency The desired frequency in Hz. Set to 0 to stop the timer.
 */
void setTimerFrequency(int timerIndex, uint32_t frequency) {
  if (timerIndex >= 0 && timerIndex < NUM_TIMERS) {
	if (frequency > 0) {
	  uint32_t period_us = 1000000 / frequency;
	  timers[timerIndex].begin(timerCallbacks[timerIndex], period_us);
	} else {
	  timers[timerIndex].end();
	}
  }
}