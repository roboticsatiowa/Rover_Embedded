/*********************************************************************
 *  ROSArduinoBridge

    A set of simple serial commands to control a differential drive
    robot and receive back sensor and odometry data. Default
    configuration assumes use of an Arduino Mega + Pololu motor
    controller shield + Robogaia Mega Encoder shield.  Edit the
    readEncoder() and setMotorSpeed() wrapper functions if using
    different motor controller or encoder method.

    Created for the Pi Robot Project: http://www.pirobot.org
    and the Home Brew Robotics Club (HBRC): http://hbrobotics.org

    Authors: Patrick Goebel, James Nugen

    Inspired and modeled after the ArbotiX driver by Michael Ferguson

    Software License Agreement (BSD License)

    Copyright (c) 2012, Patrick Goebel.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above
       copyright notice, this list of conditions and the following
       disclaimer in the documentation and/or other materials provided
       with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

// ---------------------- DEFINES ----------------------

/* Serial port baud rate */
#define BAUDRATE 57600

/* Run the PID loop at 30 times per second */
#define PID_RATE 30 // Hz

// ---------------------- INCLUDES ----------------------

#include "Arduino.h"

// macros
#include "commands.h"
#include "pinout.h"

// motor drivers
#include "motor_driver.h"
#include "encoder_driver.h"
#include "linear_actuator_driver.h"
#include "stepper_driver.h"

#include "sensors.h"
#include "pid_controller.h"

/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;

/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;

/* Stop the robot if it hasn't received a movement command
 in this number of milliseconds */
#define AUTO_STOP_INTERVAL 10000
long lastMotorCommand = AUTO_STOP_INTERVAL;

/* Variable initialization */

// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int arg_index = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;
// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];
// The arguments converted to integers
long arg1;
long arg2;

/* Clear the current command parameters */
void resetCommand()
{
  cmd = '\0';
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));

  arg1 = 0;
  arg2 = 0;
  arg = 0;
  arg_index = 0;
}

/* Run a command.  Commands are defined in commands.h */
int runCommand()
{
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch (cmd)
  {
  case GET_BAUDRATE:
    Serial.println(BAUDRATE);
    break;
  case PIN_MODE:
    if (arg2 == 0)
      pinMode(arg1, INPUT);
    else if (arg2 == 1)
      pinMode(arg1, OUTPUT);
    Serial.println("OK");
    break;
  case PING:
    Serial.println(Ping(arg1));
    break;
  case READ_ACTUATOR_POTENTIOMETER:
    Serial.print(String(analogRead(SHOULDER_POTENTIOMETER)) + " " + String(analogRead(ELBOW_POTENTIOMETER)));
    break;
#ifdef USE_SERVOS
  case SERVO_WRITE:
    servos[arg1].setTargetPosition(arg2);
    Serial.println("OK");
    break;
  case SERVO_READ:
    Serial.println(servos[arg1].getServo().read());
    break;
#endif
  case READ_ENCODER:
    Serial.println(readEncoder(arg1));
    break;
  case RESET_ENCODERS:
    resetEncoders();
    resetPID();
    Serial.println("OK");
    break;
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */

    // TODO update PID calculations to work with new drivers
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0)
    {
      setMotorSpeeds(0, 0);
      resetPID();
      moving = 0;
    }
    else
      moving = 1;
    leftPID.TargetTicksPerFrame = arg1;
    rightPID.TargetTicksPerFrame = arg2;
    Serial.println("OK");
    break;
  case MOTOR_RAW_PWM:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    resetPID();
    moving = 0; // Sneaky way to temporarily disable the PID
    setMotorSpeeds(arg1, arg2);
    Serial.println("OK");
    break;
  case UPDATE_PID:
    while (*(str = strtok_r(p, ":", &p)) != '\0')
    {
      pid_args[i] = atoi(str);
      i++;
    }
    Kp = pid_args[0];
    Kd = pid_args[1];
    Ki = pid_args[2];
    Ko = pid_args[3];
    Serial.println("OK");
    break;
  default:
    Serial.println("Invalid Command");
    break;
  case ACTUATOR_RAW_THROTTLE:
    //output_high();
    init_linear_actuator_controller();
    set_linear_actuator_speed(arg1,arg2);
    //delay(50); //atleast 50 us
    //output_low();
    Serial.println("OKIE");
    break;
  }

  return 0;
}

/* Setup function--runs once at startup. */
void setup()
{
  Serial.begin(BAUDRATE);

  // initialize linear actuator serial port
  init_linear_actuator_controller();

  // initialize stepper encoder pins as inputs
  pinMode(BASEMOTOR_ENC_A, INPUT_PULLUP);
  pinMode(BASEMOTOR_ENC_B, INPUT_PULLUP);
  pinMode(WRIST_INCLINATION_ENC_A, INPUT_PULLUP);
  pinMode(WRIST_INCLINATION_ENC_B, INPUT_PULLUP);
  pinMode(WRIST_ROTATION_ENC_A, INPUT_PULLUP);
  pinMode(WRIST_ROTATION_ENC_B, INPUT_PULLUP);
  pinMode(GRIPPER_ENC_A, INPUT_PULLUP);
  pinMode(GRIPPER_ENC_B, INPUT_PULLUP);

  // attach interrupts to encoder pins
  attachInterrupt(digitalPinToInterrupt(BASEMOTOR_ENC_A), BASEMOTOR_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BASEMOTOR_ENC_B), BASEMOTOR_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(WRIST_INCLINATION_ENC_A), WRIST_INCLINATION_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(WRIST_INCLINATION_ENC_B), WRIST_INCLINATION_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(WRIST_ROTATION_ENC_A), WRIST_ROTATION_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(WRIST_ROTATION_ENC_B), WRIST_ROTATION_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(GRIPPER_ENC_A), GRIPPER_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(GRIPPER_ENC_B), GRIPPER_ISR, CHANGE);

  // initialize potentiometer pins as inputs
  pinMode(ELBOW_POTENTIOMETER, INPUT);
  pinMode(SHOULDER_POTENTIOMETER, INPUT);

  // initialize motor driver pins as outputs
  pinMode(R_WHEEL_DIR, OUTPUT);
  pinMode(L_WHEEL_DIR, OUTPUT);
  pinMode(R_FWD_WHEEL_PUL, OUTPUT);
  pinMode(R_MID_WHEEL_PUL, OUTPUT);
  pinMode(R_BCK_WHEEL_PUL, OUTPUT);
  pinMode(L_FWD_WHEEL_PUL, OUTPUT);
  pinMode(L_MID_WHEEL_PUL, OUTPUT);
  pinMode(L_BCK_WHEEL_PUL, OUTPUT);

  // initialize stepper motor pins as outputs
  initStepperController();

  // TODO initialize servo motors
  resetPID();

  delay(5); // give some time for setup (5 milliseconds - shouldn't make a perceptible difference)

  pinMode(GLOBAL_ENABLE, OUTPUT);
  digitalWrite(GLOBAL_ENABLE, HIGH);
}

/* Enter the main loop.  Read and parse input from the serial port
   and run any valid commands. Run a PID calculation at the target
   interval and check for auto-stop conditions.
*/
void loop()
{
  while (Serial.available() > 0)
  {
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == 13)
    {
      if (arg == 1)
        argv1[arg_index] = '\0';
      else if (arg == 2)
        argv2[arg_index] = '\0';
      runCommand();
      resetCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ')
    {
      // Step through the arguments
      if (arg == 0)
        arg = 1;
      else if (arg == 1)
      {
        argv1[arg_index] = '\0';
        arg = 2;
        arg_index = 0;
      }
      continue;
    }
    else
    {
      if (arg == 0)
      {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1)
      {
        // Subsequent arguments can be more than one character
        argv1[arg_index] = chr;
        arg_index++;
      }
      else if (arg == 2)
      {
        argv2[arg_index] = chr;
        arg_index++;
      }
    }
  }
  // TODO update this code to work with the new drivers
  // If we are using base control, run a PID calculation at the appropriate intervals
  if (millis() > nextPID)
  {
    updatePID();
    nextPID += PID_INTERVAL;
  }

  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL)
  {
    setMotorSpeeds(0, 0);
    moving = 0;
  }
    setStepperSpeed(3, analogRead(SHOULDER_POTENTIOMETER) / 4);

  

}
