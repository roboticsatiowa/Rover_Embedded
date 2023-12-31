#include "linear_actuator_driver.h"
#include "pinout.h"

int lerp(int x, int a1, int b1, int a2, int b2)
{
    return a2 + (x - a1) * (b2 - a2) / (b1 - a1);
}

/*
void init_linear_actuator_controller()
{
    LINEAR_ACTUATOR_SERIAL.begin(9700);
}

void set_linear_actuator_speed(int addr, int velocity)
{
    LINEAR_ACTUATOR_SERIAL.write(addr << 7 | lerp(velocity, -255, 255, 1, 127)); //first bit is address, rest is velocity
}
*/
//idk

void init_linear_actuator_controller()
{
    LINEAR_ACTUATOR_SERIAL.begin(9600);
}

void set_linear_actuator_speed(int addr, int power)
{
    power = lerp(power, -255, 255, 1, 127);

    byte command, magnitude;
    power = constrain(power, -127, 127);
    magnitude = abs(power) >> 1;
    
    //commands 0 - 127 runs motor 1, 64 stop
    if (addr == 1)
    {
        command = power < 0 ? 63 - magnitude : 64 + magnitude;
    }

    //commands 128-255 runs motor 2, 192 stop
    else if (addr == 2)
    {
        command = power < 0 ? 191 - magnitude : 192 + magnitude;
    }
    
    command = constrain(command, 1, 254);
    LINEAR_ACTUATOR_SERIAL.write(command);  
}

//send signal to differentiate between sabertooths, sending to s2 pin like "I AM SPEAKING TO U"
void output_high()
{
    digitalWrite(LINEAR_ACTUATOR_SERIAL, HIGH);
}

void output_low()
{
    digitalWrite(LINEAR_ACTUATOR_SERIAL, LOW);
}

