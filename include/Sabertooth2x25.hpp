#ifndef SABERTOOTH2X25
#define SABERTOOTH2X25

#include <Arduino.h>

class Sabertooth2x25
{
private:
int baudrate;
HardwareSerialIMXRT *serial;

public:

Sabertooth2x25(HardwareSerialIMXRT *serial, int baudrate = 9600);
~Sabertooth2x25();

void init();
void setMotorSpeed(int motor, int speed);

};

Sabertooth2x25::Sabertooth2x25(HardwareSerialIMXRT *serial, int baudrate) {
    this->serial = serial;
    this->baudrate = baudrate;
    this->serial->begin(baudrate);
}

#endif