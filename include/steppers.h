#ifndef STEPPERS_H
#define STEPPERS_H

#define MAX_PWM 255

void initStepperController();
void setStepperSpeed(int i, int spd);

#endif // STEPPERS_H
