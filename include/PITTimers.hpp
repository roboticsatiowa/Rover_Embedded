#pragma once
#include <Arduino.h>
#include <functional>

const int NUM_TIMERS = 4;
uint32_t FREQUENCIES[NUM_TIMERS] = {10000, 15000, 20000, 25000};

using TimerCallback = std::function<void()>;
TimerCallback timerCallbacks[NUM_TIMERS] = {nullptr};

void setupPITs() {
	CCM_CCGR1 |= CCM_CCGR1_PIT(CCM_CCGR_ON);
	PIT_MCR = 0x00;

	PIT_LDVAL0 = F_BUS_ACTUAL / FREQUENCIES[0] - 1;
	PIT_TCTRL0 = PIT_TCTRL_TIE | PIT_TCTRL_TEN;

	PIT_LDVAL1 = F_BUS_ACTUAL / FREQUENCIES[1] - 1;
	PIT_TCTRL1 = PIT_TCTRL_TIE | PIT_TCTRL_TEN;

	PIT_LDVAL2 = F_BUS_ACTUAL / FREQUENCIES[2] - 1;
	PIT_TCTRL2 = PIT_TCTRL_TIE | PIT_TCTRL_TEN;

	PIT_LDVAL3 = F_BUS_ACTUAL / FREQUENCIES[3] - 1;
	PIT_TCTRL3 = PIT_TCTRL_TIE | PIT_TCTRL_TEN;

	NVIC_ENABLE_IRQ(IRQ_PIT);
}

void pit0_isr() {
	PIT_TFLG0 = 1;
	if (timerCallbacks[0]) timerCallbacks[0]();
}

void pit1_isr() {
	PIT_TFLG1 = 1;
	if (timerCallbacks[1]) timerCallbacks[1]();
}

void pit2_isr() {
	PIT_TFLG2 = 1;
	if (timerCallbacks[2]) timerCallbacks[2]();
}

void pit3_isr() {
	PIT_TFLG3 = 1;
	if (timerCallbacks[3]) timerCallbacks[3]();
}

void attachTimerCallback(int timerIndex, TimerCallback callback) {
	if (timerIndex >= 0 && timerIndex < NUM_TIMERS) {
		timerCallbacks[timerIndex] = callback;
	}
}

void setTimerFrequency(int timerIndex, uint32_t frequency) {
	if (timerIndex >= 0 && timerIndex < NUM_TIMERS) {
		FREQUENCIES[timerIndex] = frequency;

		switch(timerIndex) {
			case 0:
				PIT_TCTRL0 &= ~PIT_TCTRL_TEN;
				PIT_LDVAL0 = F_BUS_ACTUAL / frequency - 1;
				PIT_TCTRL0 |= PIT_TCTRL_TEN;
				break;
			case 1:
				PIT_TCTRL1 &= ~PIT_TCTRL_TEN;
				PIT_LDVAL1 = F_BUS_ACTUAL / frequency - 1;
				PIT_TCTRL1 |= PIT_TCTRL_TEN;
				break;
			case 2:
				PIT_TCTRL2 &= ~PIT_TCTRL_TEN;
				PIT_LDVAL2 = F_BUS_ACTUAL / frequency - 1;
				PIT_TCTRL2 |= PIT_TCTRL_TEN;
				break;
			case 3:
				PIT_TCTRL3 &= ~PIT_TCTRL_TEN;
				PIT_LDVAL3 = F_BUS_ACTUAL / frequency - 1;
				PIT_TCTRL3 |= PIT_TCTRL_TEN;
				break;
		}
	}
}