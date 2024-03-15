/***************************************************************
   Motor driver definitions

   Add a "#elif defined" block to this file to include support
   for a particular motor driver.  Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.

   *************************************************************/

#include "Arduino.h"
#include "motor_driver.h"
#include "commands.h"
#include "pinout.h"

#define MAX_PWM 255

void setMotorSpeed(int side, int spd)
{
  unsigned char reverse = 0;

  if (spd < 0)
  {
    spd = -spd;
    reverse = 1;
  }
  if (spd > MAX_PWM) 
  {
    spd = MAX_PWM;
  }
  if (side == LEFT)
  {
    analogWrite(L_FWD_WHEEL_PUL, spd);
    analogWrite(L_MID_WHEEL_PUL, spd);
    analogWrite(L_BCK_WHEEL_PUL, spd);
    digitalWrite(L_WHEEL_DIR, reverse);
  }
  else /*if (i == RIGHT) //no need for condition*/
  {
    analogWrite(R_FWD_WHEEL_PUL, spd);
    analogWrite(R_MID_WHEEL_PUL, spd);
    analogWrite(R_BCK_WHEEL_PUL, spd);
    digitalWrite(R_WHEEL_DIR, reverse);
  }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed)
{
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}
