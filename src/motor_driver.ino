/***************************************************************
   Motor driver definitions

   Add a "#elif defined" block to this file to include support
   for a particular motor driver.  Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.

   *************************************************************/

#include "Arduino.h"
#include "motor_driver.h"
#include "commands.h"

#define MAX_PWM 255

// Sets enable pin high
void initMotorController()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(RIGHT_ENABLE, OUTPUT);
  pinMode(LEFT_ENABLE, OUTPUT);

  pinMode(RIGHT_DIR, OUTPUT);
  pinMode(LEFT_DIR, OUTPUT);

  pinMode(RIGHT_FWD_MOTOR, OUTPUT);
  pinMode(RIGHT_MID_MOTOR, OUTPUT);
  pinMode(RIGHT_BCK_MOTOR, OUTPUT);

  pinMode(LEFT_FWD_MOTOR, OUTPUT);
  pinMode(LEFT_MID_MOTOR, OUTPUT);
  pinMode(LEFT_BCK_MOTOR, OUTPUT);

  digitalWrite(RIGHT_ENABLE, HIGH);
  digitalWrite(LEFT_ENABLE, HIGH);
}

void setMotorSpeed(int side, int spd)
{
  unsigned char reverse = 0;

  if (spd < 0)
  {
    spd = -spd;
    reverse = 1;
  }
  if (spd > MAX_PWM)
    spd = MAX_PWM;

  if (side == LEFT)
  {
    if (reverse == 0)
    {
      analogWrite(LEFT_FWD_MOTOR, spd);
      analogWrite(LEFT_MID_MOTOR, spd);
      analogWrite(LEFT_BCK_MOTOR, spd);
      digitalWrite(LEFT_DIR, LOW);
    }
    else if (reverse == 1)
    {
      analogWrite(LEFT_FWD_MOTOR, spd);
      analogWrite(LEFT_MID_MOTOR, spd);
      analogWrite(LEFT_BCK_MOTOR, spd);
      digitalWrite(LEFT_DIR, HIGH);
    }
  }
  else /*if (i == RIGHT) //no need for condition*/
  {
    if (reverse == 0)
    {
      analogWrite(RIGHT_FWD_MOTOR, spd);
      analogWrite(RIGHT_MID_MOTOR, spd);
      analogWrite(RIGHT_BCK_MOTOR, spd);
      digitalWrite(RIGHT_DIR, LOW);
    }
    else if (reverse == 1)
    {
      analogWrite(RIGHT_FWD_MOTOR, spd);
      analogWrite(RIGHT_MID_MOTOR, spd);
      analogWrite(RIGHT_BCK_MOTOR, spd);
      digitalWrite(RIGHT_DIR, HIGH);
    }
  }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed)
{
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}
