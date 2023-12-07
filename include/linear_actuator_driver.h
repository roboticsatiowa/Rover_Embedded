#ifndef LINEAR_ACTUATOR_DRIVER_H
#define LINEAR_ACTUATOR_DRIVER_H

#include <Arduino.h>

void init_linear_actuator_controller();
void set_linear_actuator_speed(int i, int spd);
void output_high();
void output_low();



#endif // LINEAR_ACTUATOR_DRIVER_H