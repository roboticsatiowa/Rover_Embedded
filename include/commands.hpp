
/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/
// TODO fix inconsistent naming
// TODO convert to enum

#pragma once

enum COMMANDS
{
  // Encoders
  READ_ENCODER = 'e',                // e [encoder id]
  RESET_ENCODERS = 'f',              // f
  ACTUATOR_RAW = 'h',                // h [actuator id] [throttle]
  READ_ACTUATOR_POTENTIOMETER = 'i', // i
  MOTOR_RAW = 'l',                   // l [throttle]
  STEPPER_RAW = 'o',                 // o [stepper id] [throttle]
                     // Misc commands
  HEADLIGHT_CONTROL = 'z',
  DISABLE_PINS = 'd',
  WARNING_LIGHT = 'w',
  READ_BATTERY_VOLTAGE = 'g'
};

#define READ_ENCODER 'e'                // e [encoder id]
#define RESET_ENCODERS 'f'              // f
#define ACTUATOR_RAW 'h'                // h [actuator id] [throttle]
#define READ_ACTUATOR_POTENTIOMETER 'i' // i
#define MOTOR_RAW 'l'                   // l [throttle]
#define STEPPER_RAW 'o'                 // o [stepper id] [throttle]
// Misc commands
#define UPDATE_PID 's'
#define HEADLIGHT_CONTROL 'z'

// disable pins
#define DISABLE_PINS 'd'
#define WARNING_LIGHT 'w'

#define READ_BATTERY_VOLTAGE 'g'
#define BME_SENSOR 'b'
