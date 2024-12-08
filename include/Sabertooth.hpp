#ifndef SABERTOOTH2X25
#define SABERTOOTH2X25

#include <Arduino.h>

// Sabertooth2x25 motor driver. Operated in simplified serial mode
// https://www.dimensionengineering.com/datasheets/Sabertooth2x25v2.pdf
class SabertoothDriver {
private:
    int baudrate;
    byte address; 
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
    SabertoothDriver(byte address, BAUDRATE baudrate) {
        // ensure valid baudrate. probably a better way to do this
        if (baudrate != BAUD_2400 && baudrate != BAUD_9600 && baudrate != BAUD_19200 && baudrate != BAUD_38400) {
            baudrate = BAUD_9600;
            return;
        }
        this->baudrate = baudrate;
        this->address = address; 
    }

    void init(HardwareSerialIMXRT *serial) { 
        this->serial = serial;
        this->serial->begin(baudrate);

        delay(2000); 

        // baud rate is set by sending bauding character (170 in dec). 
        serial->write(0xAA); 
    }


    /**
        * `setSpeed` sets the voltage of motor driver. Actual speed may vary due to load and battery voltage fluctuations.
        * @param motor The motor to set the speed of. 0 (L) or 1 (R). motor 1 is controlled by 0 - forward, 1 - back. motor 
        * 2 is controlled by 4 - forward and 5 - back. 
        * @param speed The speed to set the motor to. -127 to 127.
    */

    void setSpeed(uint8_t motor, int8_t speed) { 
        byte command; 
        
        if (motor != 0 || motor != 1) { 
        }

        if (motor == 0) { 
            command = 0; 
        }

        if (motor == 1) { 
            command = 4; 
        } 

        if (speed < 0) { 
            command++; 
            speed = -speed; 
        }

        speed = constrain(speed, 0, 127); 

        byte checksum = (address + command + speed) & 0b01111111;
        
        serial->write(address); 
        serial->write(command); 
        serial->write(speed); 
        serial->write(checksum); 

    }
};
#endif