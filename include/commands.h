
/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

// Encoders
#define READ_ENCODER 'e' // e [encoder id]
#define RESET_ENCODERS 'f' // f

// Actuators
#define ACTUATOR_SPEEDS 'g' // g [actuator id] [speed]
#define ACTUATOR_RAW_THROTTLE 'h' // h [actuator id] [throttle]

// actuator potentiometers
#define READ_ACTUATOR_POTENTIOMETER 'i' // i  [actuator id]

// Motors
#define MOTOR_SPEEDS   'k'
#define MOTOR_RAW_PWM  'l'

// Steppers   
#define STEPPER_SPEEDS    'm'
#define STEPPER_RAW_DUTYCYCLE 'o'

// Misc commands
#define GET_BAUDRATE   'p'
#define PIN_MODE       'q'
#define PING           'r'
#define UPDATE_PID     's'
#define HEADLIGHT_CONTROL 'z'

//disable pins
#define DISABLE_PINS 'd'

//serial port disconnected
#define SERIAL_DISCONNECTED 'g'

// disable pins 
#define DISABLE_PINS 'd'

// serial port disconnected 
#define SERIAL_DISCONNECTED 'g'

#define WARNING_LIGHT 'w'


// *Not Commands*
#define LEFT            0
#define RIGHT           1
#define MAX_PWM         255

#endif


