#include <Arduino.h>
// macros
#include "commands.h"
#include "pinout.h"

// Hardware drivers
#include "Sabertooth.hpp"
#include "Stepper.hpp"
#include "IncrementalEncoder.hpp"

#define BAUDRATE 115200           // Teensy <---> Jetson
#define AUTO_STOP_INTERVAL 1000  // milliseconds //TODO this is an absurdly high value for testing


// TODO toggleable light class
int headlight_state = 0;
long lastCmd = AUTO_STOP_INTERVAL;
int showWarning = 1;

// Motor controllers
Sabertooth *armActuators;
Sabertooth *backWheelMotors;
Sabertooth *midWheelMotors;
Sabertooth *frontWheelMotors;

// Stepper motors
Stepper *baseMotor;
Stepper *wristInclinationMotor;
Stepper *wristRotationMotor;
Stepper *gripperMotor;

// Encoders
IncrementalEncoder *baseEncoder;
IncrementalEncoder *wristInclinationEncoder;
IncrementalEncoder *wristRotationEncoder;
IncrementalEncoder *gripperEncoder;

// lists for easy iteration and indexing when parsing commands


// ---------------------- END GLOBAL VARIABLES ----------------------

void pulseLED() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1);
  digitalWrite(LED_BUILTIN, LOW);
}

// // void stopAllMotors() {
//     for (int i = 0; i < 4; i++) {
//       if (stepperMotors[i] != nullptr) {
//         stepperMotors[i]->setSpeed(0);
//       }
//     }

//     for (int i = 0; i < 3; i++) {
//       if (wheelMotors[i] != nullptr) {
//         wheelMotors[i]->setSpeed(0, 0);
//         wheelMotors[i]->setSpeed(1, 0);
//       }
//     }

//     for (int i = 0; i < 1; i++) {
//       if (actuators[i] != nullptr) {
//         actuators[i]->setSpeed(0, 0);
//         actuators[i]->setSpeed(1, 0);
//       }
//     }
// }

/* Run a command.  Commands are defined in commands.h */
int runCommand(char cmd, String args[], int numArgs) {
  lastCmd = millis();
  
  int index = args[0].toInt();

  switch (cmd) {
    case READ_ACTUATOR_POTENTIOMETER:
      Serial.print(String(analogRead(SHOULDER_POTENTIOMETER)) + " " + String(analogRead(ELBOW_POTENTIOMETER)));
      break;

    case STEPPER_RAW:

      if (index == 0) {
        baseMotor->setSpeed(args[1].toInt());
      } else if (index == 1) {
        wristInclinationMotor->setSpeed(args[1].toInt());
      } else if (index == 2) {
        wristRotationMotor->setSpeed(args[1].toInt());
      } else if (index == 3) {
        gripperMotor->setSpeed(args[1].toInt());
      }

      break;

    // case READ_ENCODER:
    //   for (int i = 0; i < 4; i++) {
    //     Serial.print(encoders[i]->getTicks());
    //     Serial.print(" ");
    //   }
    //   Serial.println();
    //   break;
    // case RESET_ENCODERS:
    //   for (int i = 0; i < 4; i++) {
    //     encoders[i]->reset();
    //   }
    //   break;

    case MOTOR_RAW:
      // TODO individual motor speeds for easier turning and such
      backWheelMotors->setSpeed(args[0].toInt(), args[1].toInt());
      midWheelMotors->setSpeed(args[0].toInt(), args[1].toInt());
      frontWheelMotors->setSpeed(args[0].toInt(), args[1].toInt());
      Serial.println(String(args[0].toInt()) + " " + String(args[1].toInt()));
      break;

    case ACTUATOR_RAW:
      armActuators->setSpeed(args[0].toInt(), args[1].toInt());
      break;

    case DISABLE_PINS:
      // stopAllMotors();
      Serial.println("OK");
      break;

    case HEADLIGHT_CONTROL:
      digitalWrite(HEADLIGHT, !headlight_state);
      headlight_state = !headlight_state;
      break;

    case WARNING_LIGHT:
      // warningLight();
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

    // invalid command empty string
    if (cmd == ' ' && chr == '\r') {
        return;
    }

    // Terminate command with a carriage return (CR)
    if (chr == '\r') {
      runCommand(cmd, args, argNum);
      Serial.println("OK");
      return;
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

    args[argNum - 1] += chr;
  }
}

// --------------------------------------------
//              Main Setup and Loop
// --------------------------------------------


void setup() {
  // Main serial port for communication with Jetson
  Serial.begin(BAUDRATE);

  // Initialize motor controllers
  armActuators = new Sabertooth(&LINEAR_ACTUATOR_SERIAL, Sabertooth::BAUD_38400); // RX=28, TX=29
  backWheelMotors = new Sabertooth(&BACK_WHEEL_SERIAL, Sabertooth::BAUD_38400); // TX=1
  midWheelMotors = new Sabertooth(&MID_WHEEL_SERIAL, Sabertooth::BAUD_38400); // TX=8
  frontWheelMotors = new Sabertooth(&FRONT_WHEEL_SERIAL, Sabertooth::BAUD_38400); //TX=24

  // Initialize stepper motor pins as outputs
  baseMotor = new Stepper(BASEMOTOR_PUL, BASEMOTOR_DIR);
  wristInclinationMotor = new Stepper(WRIST_INCLINATION_PUL, WRIST_INCLINATION_DIR);
  wristRotationMotor = new Stepper(WRIST_ROTATION_PUL, WRIST_ROTATION_DIR);
  gripperMotor = new Stepper(GRIPPER_PUL, GRIPPER_DIR); 

  // Initialize encoders
  // baseEncoder = new IncrementalEncoder(BASEMOTOR_ENC_A, BASEMOTOR_ENC_B);
  // wristInclinationEncoder = new IncrementalEncoder(WRIST_INCLINATION_ENC_A, WRIST_INCLINATION_ENC_B);
  // wristRotationEncoder = new IncrementalEncoder(WRIST_ROTATION_ENC_A, WRIST_ROTATION_ENC_B);
  // gripperEncoder = new IncrementalEncoder(GRIPPER_ENC_A, GRIPPER_ENC_B);

  // initialize potentiometer pins as inputs
  pinMode(ELBOW_POTENTIOMETER, INPUT);
  pinMode(SHOULDER_POTENTIOMETER, INPUT);

  // misc pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HEADLIGHT, OUTPUT);

  // Allow external hardware some time to boot up
  delay(100);

  // Enable all motor controllers
  // pinMode(GLOBAL_ENABLE, OUTPUT);
  // digitalWrite(GLOBAL_ENABLE, HIGH);
}

// Main loop. Arduino library will call this function repeatedly.
void loop() {

  // flash the LED when we receive a command. very useful for debugging
  if (Serial.available() > 0) {
    pulseLED();
  }

  parseSerial();


  // need to update pins manually because we are using an open collecter (common anode) configuration which isnt supported by the hardware pwm timer.
  // see https://www.omc-stepperonline.com/download/DM542T.pdf section 4 for more info on open drain configuration
  wristInclinationMotor->updatePin();
  wristRotationMotor->updatePin();
  baseMotor->updatePin();
  gripperMotor->updatePin();

  // Safety auto stop
  // if (millis() - lastCmd > AUTO_STOP_INTERVAL) {
  //   lastCmd = millis();
  //   stopAllMotors();
  //   Serial.println("STOPPING");
  // }
}