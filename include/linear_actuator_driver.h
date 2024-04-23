#ifndef LINEAR_ACTUATOR_DRIVER_H
#define LINEAR_ACTUATOR_DRIVER_H

#include <Arduino.h>

float lerp(float x, float a1, float b1, float a2, float b2);
void init_linear_actuator_controller();
void set_linear_actuator_speed(int i, int spd);



#endif // LINEAR_ACTUATOR_DRIVER_H