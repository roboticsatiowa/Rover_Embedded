#include <Arduino.h>
#include <Servo.h>
// macros
#include "commands.h"
#include "pinout.h"

// Hardware drivers
#include "Sabertooth.hpp"
#include "Stepper.hpp"
#include "IncrementalEncoder.hpp"
#include "BME280.hpp"
#include "Watchdog_t4.h"

#define BAUDRATE 115200         // Teensy <---> Jetson
#define AUTO_STOP_INTERVAL 2000 // milliseconds

#define GRIPPER_ANGLE_OPENED 0
#define GRIPPER_ANGLE_CLOSED 90
#define GRIPPER_STEP 5

int gripperAngle = GRIPPER_ANGLE_OPENED;

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

// Servos
Servo handServo;

// Encoders
IncrementalEncoder *baseEncoder;
IncrementalEncoder *wristInclinationEncoder;
IncrementalEncoder *wristRotationEncoder;
IncrementalEncoder *gripperEncoder;

BME280 bme;

// Watchdog
WDT_T4<WDT1> wdt;

// lists for easy iteration and indexing when parsing commands

// ---------------------- END GLOBAL VARIABLES ----------------------

void stopAllMotors()
{
  armActuators->setSpeed(0, 0);
  backWheelMotors->setSpeed(0, 0);
  midWheelMotors->setSpeed(0, 0);
  frontWheelMotors->setSpeed(0, 0);

  baseMotor->setSpeed(0);
  wristInclinationMotor->setSpeed(0);
  wristRotationMotor->setSpeed(0);
}

void setCameraMountSpeed(int direction)
{
  if (direction > 0)
  {
    digitalWrite(CAMERA_MOUNT_MOTOR_IN1, HIGH);
    digitalWrite(CAMERA_MOUNT_MOTOR_IN2, LOW);
  }
  else if (direction < 0)
  {
    digitalWrite(CAMERA_MOUNT_MOTOR_IN1, LOW);
    digitalWrite(CAMERA_MOUNT_MOTOR_IN2, HIGH);
  }
  else
  {
    digitalWrite(CAMERA_MOUNT_MOTOR_IN1, LOW);
    digitalWrite(CAMERA_MOUNT_MOTOR_IN2, LOW);
  }
}

/* Run a command.  Commands are defined in commands.h */
int runCommand(char cmd, String args[], int numArgs)
{
  lastCmd = millis();

  int index = args[0].toInt();

  switch (cmd)
  {
  case READ_ACTUATOR_POTENTIOMETER:
    Serial.print(String(analogRead(SHOULDER_POTENTIOMETER)) + " " + String(analogRead(ELBOW_POTENTIOMETER)));
    break;

  case STEPPER_RAW:
    if (index == 0) {
        baseMotor->setSpeed(args[1].toInt());
    }
    else if (index == 1) {
        wristInclinationMotor->setSpeed(args[1].toInt());
    }
    else if (index == 2) {
        wristRotationMotor->setSpeed(args[1].toInt());
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
    // Serial.println(String(args[0].toInt()) + " " + String(args[1].toInt()));
    break;

  case ACTUATOR_RAW:
    armActuators->setSpeed(args[0].toInt(), args[1].toInt());
    break;

  case DISABLE_PINS:
    stopAllMotors();
    break;

  case WARNING_LIGHT:
    // warningLight();
    break;

  case BME_SENSOR:
    Serial.println(bme.getTemperature());
    Serial.println(bme.getPressure());
    Serial.println(bme.getHumidity());
    break;

  case READ_BATTERY_VOLTAGE:
    Serial.println(((analogRead(BATTERY_VOLTAGE) * 5.0) / 1024.0) * (7500.0 + 30000.0) / 7500.0, 2);
    break;

  case HAND_SERVO:
  {
    int gripperArg = args[0].toInt();

    if (gripperArg == 1)
    {
        gripperAngle -= GRIPPER_STEP;
        if (gripperAngle < GRIPPER_ANGLE_OPENED)
            gripperAngle = GRIPPER_ANGLE_OPENED;
        handServo.write(gripperAngle);
        Serial.print("Opening gripper: ");
        Serial.println(gripperAngle);
    }
    else if (gripperArg == -1)
    {
        gripperAngle += GRIPPER_STEP;
        if (gripperAngle > GRIPPER_ANGLE_CLOSED)
            gripperAngle = GRIPPER_ANGLE_CLOSED;
        handServo.write(gripperAngle);
        Serial.print("Closing gripper: ");
        Serial.println(gripperAngle);
    }

    break;
  }

  // TODO: ADD in the headlight command and camera mount commands
  case CAMERA_MOUNT:
    setCameraMountSpeed(args[0].toInt());
    break;

  // case HEADLIGHT_CONTROL:
  //  headlight_state = !headlight_state;
  //  digitalWrite(LED_HEADLIGHT, headlight_state);
  //  break;

  default:
    Serial.println("Invalid Command");
    break;
  }

  return 0;
}

void parseSerial()
{
  char cmd = ' ';

  String args[4];
  for (int i = 0; i < 4; i++)
  {
    args[i] = "";
  }

  int argNum = 0;
  while (Serial.available() > 0)
  {
    char chr = Serial.read();

    // invalid command empty string
    if (cmd == ' ' && chr == '\r')
    {
      return;
    }

    // Terminate command with a carriage return (CR)
    if (chr == '\r')
    {
      runCommand(cmd, args, argNum);
      Serial.println("OK");
      return;
    }

    // Use spaces to delimit parts of the command
    if (chr == ' ')
    {
      argNum++;
      continue;
    }

    // Store the command character
    if (argNum == 0)
    {
      cmd = chr;
      continue;
    }

    args[argNum - 1] += chr;
  }
}

// --------------------------------------------
//              Main Setup and Loop
// --------------------------------------------

void setup()
{
  // Main serial port for communication with Jetson
  Serial.begin(BAUDRATE);

  // Initialize motor controllers
  armActuators = new Sabertooth(&LINEAR_ACTUATOR_SERIAL, Sabertooth::BAUD_38400); // RX=28, TX=29
  backWheelMotors = new Sabertooth(&BACK_WHEEL_SERIAL, Sabertooth::BAUD_38400);   // TX=1
  midWheelMotors = new Sabertooth(&MID_WHEEL_SERIAL, Sabertooth::BAUD_38400);     // TX=8
  frontWheelMotors = new Sabertooth(&FRONT_WHEEL_SERIAL, Sabertooth::BAUD_38400); // TX=24

  // Initialize stepper motor pins as outputs
  baseMotor = new Stepper(BASEMOTOR_PUL, BASEMOTOR_DIR);
  wristInclinationMotor = new Stepper(WRIST_INCLINATION_PUL, WRIST_INCLINATION_DIR);
  wristRotationMotor = new Stepper(WRIST_ROTATION_PUL, WRIST_ROTATION_DIR);

  // HAnd servo setup/initializing
  handServo.attach(HAND_GRIPPER_SERVO);
  gripperAngle = GRIPPER_ANGLE_OPENED;
  handServo.write(gripperAngle);

  // Initialize encoders
  // baseEncoder = new IncrementalEncoder(BASEMOTOR_ENC_A, BASEMOTOR_ENC_B);
  // wristInclinationEncoder = new IncrementalEncoder(WRIST_INCLINATION_ENC_A, WRIST_INCLINATION_ENC_B);
  // wristRotationEncoder = new IncrementalEncoder(WRIST_ROTATION_ENC_A, WRIST_ROTATION_ENC_B);
  // gripperEncoder = new IncrementalEncoder(GRIPPER_ENC_A, GRIPPER_ENC_B);

  // initialize potentiometer pins as inputs
  pinMode(ELBOW_POTENTIOMETER, INPUT);
  pinMode(SHOULDER_POTENTIOMETER, INPUT);

  // initialize camera mount pins as outputs
  pinMode(CAMERA_MOUNT_MOTOR_IN1, OUTPUT);
  pinMode(CAMERA_MOUNT_MOTOR_IN2, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  // misc pins
  pinMode(LED_HEADLIGHT, OUTPUT);

  pinMode(BATTERY_VOLTAGE, INPUT);

  digitalWrite(LED_HEADLIGHT, LOW);

  // Allow external hardware some time to boot up
  delay(100);

  // BME sensor setup
  Wire.begin();
  // pinMode(GLOBAL_ENABLE, OUTPUT);
  // digitalWrite(GLOBAL_ENABLE, HIGH);

  // Start Watchdog
  WDT_timings_t config;
  config.timeout = 2; /* in seconds, 0->128 */
  wdt.begin(config);
}

// Main loop. Arduino library will call this function repeatedly.
void loop()
{

  parseSerial();

  // need to update pins manually because we are using an open collecter (common anode) configuration which isnt supported by the hardware pwm timer.
  // see https://www.omc-stepperonline.com/download/DM542T.pdf section 4 for more info on open drain configuration

  // TODO manually updating pins work fine for now but will be severely impacted by any blocking code. need to look into a better solution
  wristInclinationMotor->updatePin();
  wristRotationMotor->updatePin();
  baseMotor->updatePin();

  // Safety auto stop
  // if (millis() - lastCmd > AUTO_STOP_INTERVAL) {
  //   lastCmd = millis();
  //   stopAllMotors();
  //   Serial.println("STOPPING");
  // }

  // Feed Watchdog
  wdt.feed();
}
