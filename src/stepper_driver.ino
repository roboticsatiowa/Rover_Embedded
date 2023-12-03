#include "stepper_driver.h"
#include "pinout.h"

void initStepperController()
{
    // TODO set correct pins to outputs
    pinMode(BASEMOTOR_DIR, OUTPUT);
    pinMode(BASEMOTOR_PUL, OUTPUT);
    pinMode(WRIST_INCLINATION_DIR, OUTPUT);
    pinMode(WRIST_INCLINATION_PUL, OUTPUT);
    pinMode(WRIST_ROTATION_DIR, OUTPUT);
    pinMode(WRIST_ROTATION_PUL, OUTPUT);
    pinMode(GRIPPER_DIR, OUTPUT);
    pinMode(GRIPPER_PUL, OUTPUT);

    analogWrite(33, 128);
}

void setStepperSpeed(int i, int spd)
{

    if (spd > 255)
    {
        spd = 255;
    }
    else if (spd < 10) // speed becomes unstable below a certain threshold
    {
        spd = 0;
    }

    spd *= 8; // stepper unstable past 2048 Hz

    switch (i)
    {
    case 0:
        analogWriteFrequency(BASEMOTOR_PUL, spd);
        analogWrite(BASEMOTOR_PUL, (spd > 10 ? 128 : 0));
        break;
    case 1:
        analogWriteFrequency(WRIST_INCLINATION_PUL, spd);
        analogWrite(WRIST_INCLINATION_PUL, (spd > 10 ? 128 : 0));
        break;
    case 2:
        analogWriteFrequency(WRIST_ROTATION_PUL, spd);
        analogWrite(WRIST_ROTATION_PUL, (spd > 10 ? 128 : 0));
        break;
    case 3:
        analogWriteFrequency(GRIPPER_PUL, spd); 
        analogWrite(GRIPPER_PUL, (spd > 10 ? 128 : 0));
        break;
    default:
        break;
    }
}
