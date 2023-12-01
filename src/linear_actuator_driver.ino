#include "linear_actuator_driver.h"
#include "pinout.h"

int lerp(int x, int a1, int b1, int a2, int b2)
{
    return a2 + (x - a1) * (b2 - a2) / (b1 - a1);
}

void init_linear_actuator_controller()
{
    LINEAR_ACTUATOR_SERIAL.begin(9700);
}

void set_linear_actuator_speed(int addr, int velocity)
{
    LINEAR_ACTUATOR_SERIAL.write(addr << 7 | lerp(velocity, -255, 255, 1, 127)); //first bit is address, rest is velocity
}