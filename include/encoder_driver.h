/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */

#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H


long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
void ISR(int addr);
void BASEMOTOR_ISR();
void WRIST_INCLINATION_ISR();
void WRIST_ROTATION_ISR();
void GRIPPER_ISR();

#endif