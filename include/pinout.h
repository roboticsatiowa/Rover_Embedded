#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

#define GLOBAL_ENABLE 0

// ---------------- Stepper Motor Pin Assignments (May be replaced by servos in the future) ----------------

#define BASEMOTOR_PUL 19
#define BASEMOTOR_DIR 20 
#define BASEMOTOR_ENC_A 21
#define BASEMOTOR_ENC_B 22

#define WRIST_INCLINATION_PUL 15
#define WRIST_INCLINATION_DIR 16
#define WRIST_INCLINATION_ENC_A 17
#define WRIST_INCLINATION_ENC_B 18

#define WRIST_ROTATION_PUL 37
#define WRIST_ROTATION_DIR 38
#define WRIST_ROTATION_ENC_A 39
#define WRIST_ROTATION_ENC_B 40

#define GRIPPER_PUL 33
#define GRIPPER_DIR 34 
#define GRIPPER_ENC_A 35
#define GRIPPER_ENC_B 36  

// ---------------- Brushless Motor Pin Assignments ----------------

#define L_FWD_WHEEL_PUL 1 
#define L_MID_WHEEL_PUL 2
#define L_BCK_WHEEL_PUL 3
#define L_WHEEL_DIR 4

#define R_FWD_WHEEL_PUL 5
#define R_MID_WHEEL_PUL 6
#define R_BCK_WHEEL_PUL 7
#define R_WHEEL_DIR 8

// ---------------- Linear Actuator Pin Assignments ----------------

#define LINEAR_ACTUATOR_SERIAL Serial7

// ---------------- Potentiometer Pin Assignments ----------------

#define SHOULDER_POTENTIOMETER PIN_A12
#define ELBOW_POTENTIOMETER PIN_A13

// ---------------- Misc Pin Assignments ----------------

#define STATUS_LED 13
#define HEADLIGHT 9

#endif