#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <limits.h>
#include <cstdio>

#include "MockStepper.hpp"

using namespace ::testing;

TEST(StepperTest, InitTest) {
	MockStepper stepper(1, 2);

	EXPECT_CALL(stepper, initPin(1));
	EXPECT_CALL(stepper, initPin(2));

	stepper.init();
}

TEST(StepperTest, ForwardTest) {
	MockStepper stepper(1, 2);
	stepper.init();

	stepper.setSpeed(200);

	// timer hasn't been waiting long enough just yet
	EXPECT_CALL(stepper, getPulseTimer).WillOnce(Return(120));
	// Times(0), i.e. expect NOT to be called
	EXPECT_CALL(stepper, setPulseTimer).Times(0);
	EXPECT_CALL(stepper, stepperToggleFast).Times(0);

	stepper.updatePin();

	// Pick whatever value we want for the pulse timer
	// This is the period for speed == 200, plus 1
	EXPECT_CALL(stepper, getPulseTimer).WillOnce(Return(125));
	EXPECT_CALL(stepper, setPulseTimer(0));
	EXPECT_CALL(stepper, stepperToggleFast(1));

	stepper.updatePin();
}

#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

	::testing::InitGoogleMock();
}

void loop()
{
	// Run tests
	if (RUN_ALL_TESTS())
	;

	// sleep for 1 sec
	delay(1000);
}

#else

int main(int argc, char **argv)
{
	::testing::InitGoogleMock(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}

#endif