#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

// ---------------- Hardware Address Assignments - *These are not pin numbers* ----------------

#define BASEMOTOR 0
#define WRIST_INCLINATION 1
#define WRIST_ROTATION 2
#define GRIPPER 3
#define SHOULDER_ACTUATOR 4
#define ELBOW_ACTUATOR 5

// ---------------- Stepper Motor Pin Assignments ----------------

#define BASEMOTOR_PWM 9 // TODO arbitrarily assigned pin number
#define BASEMOTOR_DIR 10 // TODO arbitrarily assigned pin number
#define BASEMOTOR_ENABLE 11 // TODO arbitrarily assigned pin number

#define WRIST_INCLINATION_PWM 12 // TODO arbitrarily assigned pin number
#define WRIST_INCLINATION_DIR 13 // TODO arbitrarily assigned pin number
#define WRIST_INCLINATION_ENABLE 14 // TODO arbitrarily assigned pin number

#define WRIST_ROTATION_PWM 15 // TODO arbitrarily assigned pin number
#define WRIST_ROTATION_DIR 16 // TODO arbitrarily assigned pin number
#define WRIST_ROTATION_ENABLE 17 // TODO arbitrarily assigned pin number

#define GRIPPER_PWM 18 // TODO arbitrarily assigned pin number
#define GRIPPER_DIR 19 // TODO arbitrarily assigned pin number
#define GRIPPER_ENABLE 20 // TODO arbitrarily assigned pin number

// ---------------- Encoder Pin Assignments ----------------
// Each stepper motor corresponds to 2 encoder pins, A and B

#define BASEMOTOR_ENC_A 2  // TODO arbitrarily assigned pin number
#define BASEMOTOR_ENC_B 3  // TODO arbitrarily assigned pin number

#define WRIST_INCLINATION_ENC_A 4  // TODO arbitrarily assigned pin number
#define WRIST_INCLINATION_ENC_B 5  // TODO arbitrarily assigned pin number

#define WRIST_ROTATION_ENC_A 6  // TODO arbitrarily assigned pin number
#define WRIST_ROTATION_ENC_B 7  // TODO arbitrarily assigned pin number

#define GRIPPER_ENC_A 8  // TODO arbitrarily assigned pin number
#define GRIPPER_ENC_B 9  // TODO arbitrarily assigned pin number


// ---------------- Brushless Motor Pin Assignments ----------------

#define RIGHT_FWD_MOTOR 2 // TODO arbitrarily assigned pin number 
#define RIGHT_MID_MOTOR 3 // TODO arbitrarily assigned pin number
#define RIGHT_BCK_MOTOR 4 // TODO arbitrarily assigned pin number

#define LEFT_FWD_MOTOR 5 // TODO arbitrarily assigned pin number
#define LEFT_MID_MOTOR 6 // TODO arbitrarily assigned pin number
#define LEFT_BCK_MOTOR 7 // TODO arbitrarily assigned pin number

#define RIGHT_DIR 22 // TODO arbitrarily assigned pin number
#define LEFT_DIR 23 // TODO arbitrarily assigned pin number

#define RIGHT_ENABLE 24 // TODO arbitrarily assigned pin number
#define LEFT_ENABLE 25 // TODO arbitrarily assigned pin number


// ---------------- Linear Actuator Pin Assignments ----------------

#define LINEAR_ACTUATOR_TX Serial1 // TODO arbitrarily assigned serial port number

// ---------------- Potentiometer Pin Assignments ----------------

#define SHOULDER_POTENTIOMETER A0 // TODO arbitrarily assigned pin number
#define ELBOW_POTENTIOMETER A1 // TODO arbitrarily assigned pin number

#endif