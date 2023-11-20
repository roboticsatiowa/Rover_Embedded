/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

// Manual pin control. Do not use in production
#define ANALOG_READ    'a'
#define ANALOG_WRITE   'b'
#define DIGITAL_READ   'c'
#define DIGITAL_WRITE  'd'

// Encoders
#define READ_ENCODERS  'e'
#define RESET_ENCODERS 'f'

// Actuators
#define ACTUATOR_SPEEDS 'g'
#define ACTUATOR_RAW_THROTTLE 'h'

// actuator potentiometers
#define ACTUATOR_POTENTIONMETER_READ 'i'

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

// *Not Commands*
#define LEFT            0
#define RIGHT           1
#define MAX_PWM         255

#endif


