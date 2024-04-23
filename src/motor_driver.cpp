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
#include "linear_actuator_driver.h"

   // #define MAX_PWM 255
   // void setMotorSpeed(int side, int spd)
   // {
   //   unsigned char reverse = 0;

   //   if (spd < 0)
   //   {
   //     spd = -spd;
   //     reverse = 1;
   //   }
   //   if (spd > MAX_PWM) 
   //   {
   //     spd = MAX_PWM;
   //   }
   //   if (side == LEFT)
   //   {
   //     analogWrite(L_FWD_WHEEL_PUL, spd);
   //     analogWrite(L_MID_WHEEL_PUL, spd);
   //     analogWrite(L_BCK_WHEEL_PUL, spd);
   //     digitalWrite(L_WHEEL_DIR, reverse);
   //   }
   //   else /*if (i == RIGHT) //no need for condition*/
   //   {
   //     analogWrite(R_FWD_WHEEL_PUL, spd);
   //     analogWrite(R_MID_WHEEL_PUL, spd);
   //     analogWrite(R_BCK_WHEEL_PUL, spd);
   //     digitalWrite(R_WHEEL_DIR, reverse);
   //   }
   // }

   // void setMotorSpeeds(int leftSpeed, int rightSpeed)
   // {
   //   setMotorSpeed(LEFT, leftSpeed);
   //   setMotorSpeed(RIGHT, rightSpeed);
   // }

void initMotorControllers() {
  Serial1.begin(38400);
  Serial2.begin(38400);
  Serial6.begin(38400);
}

void setMotorSpeed(int driverNum, int addr, int velocity) {

  switch (driverNum)
  {
  case 0:
    Serial1.write(addr << 7 | (int)lerp(velocity, -255, 255, 1, 127)); //first bit is address, rest is velocity
    break;
  case 1:
    Serial2.write(addr << 7 | (int)lerp(velocity, -255, 255, 1, 127)); //first bit is address, rest is velocity
    break;
  case 2:
    Serial6.write(addr << 7 | (int)lerp(velocity, -255, 255, 1, 127)); //first bit is address, rest is velocity
    break;
  default:
    break;
  }
}
