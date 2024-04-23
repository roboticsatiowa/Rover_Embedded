#include "stepper_driver.h"
#include "pinout.h"

void initStepperController()
{
    pinMode(BASEMOTOR_DIR, OUTPUT);
    pinMode(BASEMOTOR_PUL, OUTPUT);
    pinMode(WRIST_INCLINATION_DIR, OUTPUT);
    pinMode(WRIST_INCLINATION_PUL, OUTPUT);
    pinMode(WRIST_ROTATION_DIR, OUTPUT);
    pinMode(WRIST_ROTATION_PUL, OUTPUT);
    pinMode(GRIPPER_DIR, OUTPUT);
    pinMode(GRIPPER_PUL, OUTPUT);
}

void setStepperSpeed(int i, int spd)
{
    if (spd > 255)
    {
        spd = 255;
    }
    else if (spd < -255)
    {
        spd = -255;
    }

    int dir = spd < 0 ? 1 : 0;

    if (spd < 0)
    {
        spd *= -1;
    }

    spd *= 8;

    switch (i)
    {
    case 0:
        analogWriteFrequency(BASEMOTOR_PUL, spd);
        analogWrite(BASEMOTOR_PUL, (spd != 0 ? 128 : 0));
        digitalWrite(BASEMOTOR_DIR, dir);
        break;
    case 1:
        analogWriteFrequency(WRIST_INCLINATION_PUL, spd);
        analogWrite(WRIST_INCLINATION_PUL, (spd != 0 ? 128 : 0));
        digitalWrite(WRIST_INCLINATION_DIR, dir);
        break;
    case 2:
        analogWriteFrequency(WRIST_ROTATION_PUL, spd);
        analogWrite(WRIST_ROTATION_PUL, (spd != 0 ? 128 : 0));
        digitalWrite(WRIST_ROTATION_DIR, dir);
        break;
    case 3:
        analogWriteFrequency(GRIPPER_PUL, spd);
        analogWrite(GRIPPER_PUL, (spd != 0 ? 128 : 0));
        digitalWrite(GRIPPER_DIR, dir);
        break;
    default:
        break;
    }
}

