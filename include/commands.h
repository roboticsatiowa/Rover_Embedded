/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

// Manual pin control. Do not use in production
#define ANALOG_READ    'a'
#define ANALOG_WRITE   'x'
#define DIGITAL_READ   'd'
#define DIGITAL_WRITE  'w'

// Encoders
#define READ_ENCODERS  'e'
#define RESET_ENCODERS 'r'

// Motors
#define MOTOR_SPEEDS   'm'
#define MOTOR_RAW_PWM  'o'

// Servos   
#define SERVO_WRITE    's'
#define SERVO_READ     't'

// Misc commands
#define GET_BAUDRATE   'b'
#define PIN_MODE       'c'
#define PING           'p'
#define UPDATE_PID     'u'

// *Not Commands*
#define LEFT            0
#define RIGHT           1

#endif


