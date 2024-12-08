#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

// TODO convert to enums

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

// ---------------- Serial Port Assignments ----------------
#define LINEAR_ACTUATOR_SERIAL Serial7  // RX=28, TX=29
#define BACK_WHEEL_SERIAL Serial7       // RX=0,  TX=1
#define MID_WHEEL_SERIAL Serial7        // RX=7,  TX=8
#define FRONT_WHEEL_SERIAL Serial7      // RX=25, TX=24

// #define LINEAR_ACTUATOR_SERIAL Serial7  // RX=28, TX=29
// #define BACK_WHEEL_SERIAL Serial1       // RX=0,  TX=1
// #define MID_WHEEL_SERIAL Serial2        // RX=7,  TX=8
// #define FRONT_WHEEL_SERIAL Serial6      // RX=25, TX=24

// ---------------- Potentiometer Pin Assignments ----------------

#define SHOULDER_POTENTIOMETER PIN_A12
#define ELBOW_POTENTIOMETER PIN_A13

#define BATTERY_VOLTAGE PIN_A4

// ---------------- Misc Pin Assignments ----------------

#define HEADLIGHT 9

#endif