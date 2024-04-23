#include "linear_actuator_driver.h"
#include "pinout.h"

// Sabertooth dipswitch configuration: 101001

float lerp(float x, float a1, float b1, float a2, float b2)
{
    return a2 + (x - a1) * (b2 - a2) / (b1 - a1);
}


void init_linear_actuator_controller()
{
    LINEAR_ACTUATOR_SERIAL.begin(38400);
}

void set_linear_actuator_speed(int addr, int velocity)
{
   LINEAR_ACTUATOR_SERIAL.write(addr << 7 | (int)lerp(velocity, -255, 255, 1, 127)); //first bit is address, rest is velocity
    //does this work for motor 2?
}