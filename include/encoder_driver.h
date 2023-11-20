/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */

#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

// below can be changed, but should be PORTD pins;
// otherwise additional changes in the code are required
#define LEFT_ENC_PIN_A 2  // TODO arbitrarily assigned pin number
#define LEFT_ENC_PIN_B 3  // TODO arbitrarily assigned pin number

// below can be changed, but should be PORTC pins
#define RIGHT_ENC_PIN_A 4  // TODO arbitrarily assigned pin number
#define RIGHT_ENC_PIN_B 5  // TODO arbitrarily assigned pin number

long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();

#endif