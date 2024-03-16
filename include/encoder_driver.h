#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

long readEncoder(int i);
void resetEncoder(int i);
void resetAllEncoders();

// Interrupt Service Routine
void ISR(int addr);

// Interrupt Service Routines
void BASEMOTOR_ISR();
void WRIST_INCLINATION_ISR();
void WRIST_ROTATION_ISR();
void GRIPPER_ISR();

#endif