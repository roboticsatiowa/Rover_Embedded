#ifndef SABERTOOTH2X25
#define SABERTOOTH2X25

#include <Arduino.h>

// Sabertooth2x25 motor driver. Operated in simplified serial mode
// https://www.dimensionengineering.com/datasheets/Sabertooth2x25v2.pdf
class Sabertooth {
private:
    int baudrate;
    HardwareSerialIMXRT *serial;
    float lerp(float x, float a1, float b1, float a2, float b2){return a2 + (x - a1) * (b2 - a2) / (b1 - a1);}

public:
    /**
     * Baudrate options for Sabertooth2x25 motor driver. 
     * 2400, 9600, 19200, 38400
    */
    enum BAUDRATE {BAUD_2400 = 2400,BAUD_9600 = 9600,BAUD_19200 = 19200,BAUD_38400 = 38400};
    /**
        * Constructor for Sabertooth2x25 motor driver. Automatically initializes serial port.
        * @param serial The serial port to use
        * @param baudrate The baudrate to use. Valid values are 2400, 9600, 19200, 38400
    */
    Sabertooth(HardwareSerialIMXRT *serial, BAUDRATE baudrate) {
        // ensure valid baudrate. probably a better way to do this
        if (baudrate != BAUD_2400 && baudrate != BAUD_9600 && baudrate != BAUD_19200 && baudrate != BAUD_38400) {
            baudrate = BAUD_9600;
            return;
        }

        this->serial = serial;
        this->baudrate = baudrate;
        this->serial->begin(baudrate);
    }
    /**
        * `setSpeed` sets the voltage of motor driver. Actual speed may vary due to load and battery voltage fluctuations.
        * @param motor The motor to set the speed of. 0 (L) or 1 (R)
        * @param speed The speed to set the motor to. -255 to 255
    */
    void setSpeed(int motor, int speed) {
        speed = constrain(speed, -255, 255);
        serial->write(motor << 7 | (int)lerp(speed, -255, 255, 1, 127));
    }
};
#endif