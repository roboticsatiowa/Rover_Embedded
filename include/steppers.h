#ifndef STEPPERS_H
#define STEPPERS_H

#define MAX_PWM 255

#define BASEMOTOR = 0
#define BASEMOTOR_PWM 9 // TODO arbitrarily assigned pin number
#define BASEMOTOR_DIR 10 // TODO arbitrarily assigned pin number
#define BASEMOTOR_ENABLE 11 // TODO arbitrarily assigned pin number

#define WRIST_INCLINATION 1
#define WRIST_INCLINATION_PWM 12 // TODO arbitrarily assigned pin number
#define WRIST_INCLINATION_DIR 13 // TODO arbitrarily assigned pin number
#define WRIST_INCLINATION_ENABLE 14 // TODO arbitrarily assigned pin number

#define WRIST_ROTATION 2
#define WRIST_ROTATION_PWM 15 // TODO arbitrarily assigned pin number
#define WRIST_ROTATION_DIR 16 // TODO arbitrarily assigned pin number
#define WRIST_ROTATION_ENABLE 17 // TODO arbitrarily assigned pin number

#define GRIPPER 3
#define GRIPPER_PWM 18 // TODO arbitrarily assigned pin number
#define GRIPPER_DIR 19 // TODO arbitrarily assigned pin number
#define GRIPPER_ENABLE 20 // TODO arbitrarily assigned pin number

void initStepperController();
void setStepperSpeed(int i, int spd);

#endif // STEPPERS_H
