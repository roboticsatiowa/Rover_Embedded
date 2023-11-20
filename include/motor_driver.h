/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#define RIGHT_FWD_MOTOR 2 // TODO arbitrarily assigned pin number 
#define RIGHT_MID_MOTOR 3 // TODO arbitrarily assigned pin number
#define RIGHT_BCK_MOTOR 4 // TODO arbitrarily assigned pin number

#define LEFT_FWD_MOTOR 5 // TODO arbitrarily assigned pin number
#define LEFT_MID_MOTOR 6 // TODO arbitrarily assigned pin number
#define LEFT_BCK_MOTOR 7 // TODO arbitrarily assigned pin number

#define RIGHT_DIR 22 // TODO arbitrarily assigned pin number
#define LEFT_DIR 23 // TODO arbitrarily assigned pin number

#define RIGHT_ENABLE 24 // TODO arbitrarily assigned pin number
#define LEFT_ENABLE 25 // TODO arbitrarily assigned pin number

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);

#endif