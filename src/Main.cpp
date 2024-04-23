#include "Arduino.h"
// macros
#include "commands.h"
#include "pinout.h"

// motor drivers
#include "motor_driver.h"
#include "encoder_driver.h"
#include "linear_actuator_driver.h"
#include "stepper_driver.h"

// ---------------------- CONSTANTS ----------------------

#define BAUDRATE 115200           // Teensy <---> Jetson
#define AUTO_STOP_INTERVAL 10000  // milliseconds   

// ---------------------- VARIABLES ----------------------
// this is pretty shit practice we should get rid of these
// lots of global variables = bad

long lastMotorCommand = AUTO_STOP_INTERVAL;

// Headlight global
int headlight_state = 0;

// warning global
int showWarning = 1;

// ---------------------- END GLOBAL VARIABLES ----------------------


/* Run a command.  Commands are defined in commands.h */
// this doesnt seem right
int runCommand(char cmd, String args[], int numArgs) {

  switch (cmd) {
    case READ_ACTUATOR_POTENTIOMETER:
      Serial.print(String(analogRead(SHOULDER_POTENTIOMETER)) + " " + String(analogRead(ELBOW_POTENTIOMETER)));
      break;

    case STEPPER_RAW:
      setStepperSpeed(args[0].toInt(), args[1].toInt());
      Serial.println("OK");
      break;

    case READ_ENCODER:
      Serial.println(readEncoder(args[0].toInt()));
      break;

    case RESET_ENCODERS:
      resetAllEncoders();
      break;

    case MOTOR_RAW:
      lastMotorCommand = millis();
      setMotorSpeed(0, args[0].toInt(), args[1].toInt());
      setMotorSpeed(1, args[0].toInt(), args[1].toInt());
      setMotorSpeed(2, args[0].toInt(), args[1].toInt());
      Serial.println(String(args[0].toInt()) + " " + String(args[1].toInt()));
      break;

    case ACTUATOR_RAW:
      init_linear_actuator_controller();
      set_linear_actuator_speed(args[0].toInt(), args[1].toInt());
      break;

    case DISABLE_PINS:
      CORE_PIN20_CONFIG = 0;
      CORE_PIN19_CONFIG = 0;
      CORE_PIN16_CONFIG = 0;
      CORE_PIN15_CONFIG = 0;
      CORE_PIN38_CONFIG = 0;
      CORE_PIN37_CONFIG = 0;
      CORE_PIN34_CONFIG = 0;
      CORE_PIN33_CONFIG = 0;
      Serial.println("DISABLE");
      break;

    case HEADLIGHT_CONTROL:
      digitalWrite(HEADLIGHT, !headlight_state);
      headlight_state = !headlight_state;
      Serial.println("OK");
      break;

    case WARNING_LIGHT:
      // warningLight();
      Serial.println("OK");
      break;
    default:
      Serial.println("Invalid Command");
      break;
  }

  return 0;
}

void parseSerial() {

  char cmd = ' ';

  String args[4];
  for (int i = 0; i < 4; i++) {
    args[i] = "";
  }

  int argNum = 0;
  
  while (Serial.available() > 0) {

    char chr = Serial.read();

    // Terminate command with a carriage return (CR)
    if (chr == '\r') {
      if (cmd == ' ') {

        continue;
      }
      runCommand(cmd, args, argNum);
      continue;
    }

    // Use spaces to delimit parts of the command
    if (chr == ' ') {
      argNum++;
      continue;
    }

    // Store the command character
    if (argNum == 0) {
      cmd = chr;
      continue;
    }

    // Store the argument characters
    args[argNum - 1] += chr;
  }
}

/* Setup function--runs once at startup. */
// TODO these should be refactored together into their own functions
void setup() {
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
  attachInterrupt(digitalPinToInterrupt(BASEMOTOR_ENC_B), *BASEMOTOR_ISR, CHANGE);
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

  initMotorControllers();

  //led pin
  pinMode(13, OUTPUT);

  // initialize headlight pin as output
  pinMode(HEADLIGHT, OUTPUT);

  // initialize stepper motor pins as outputs
  initStepperController();

  delay(50); // wait for the motor controllers to initialize
  pinMode(GLOBAL_ENABLE, OUTPUT);
  digitalWrite(GLOBAL_ENABLE, HIGH);
}

// Main loop. Arduino library will call this function repeatedly.
void loop() {

  // flash the LED when we receive a command. very useful for debugging
  if (Serial.available() > 0) {
    digitalWrite(LED_PIN, HIGH);
  }

  parseSerial();

  delay(1);
  digitalWrite(LED_PIN, LOW);
}