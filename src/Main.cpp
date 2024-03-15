#define BAUDRATE 115200 // Teensy <---> Jetson
#define PID_RATE 30 // Hz
#define AUTO_STOP_INTERVAL 10000 // milliseconds

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

// sensors
#include "sensors.h"
#include "pid_controller.h"

const int PID_INTERVAL = 1000 / PID_RATE; //milliseconds
unsigned long nextPID = PID_INTERVAL;

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
// Headlight global
int headlight_state = 0;

// warning global 
int showWarning = 1; 

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
  case STEPPER_RAW_DUTYCYCLE:
    setStepperSpeed(arg1, arg2);
    Serial.println("OK");
    break;
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
  case ACTUATOR_RAW_THROTTLE:
    init_linear_actuator_controller();
    set_linear_actuator_speed(arg1,arg2);
    Serial.println("OKIE");
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
      warningLight(); 
      Serial.println("OK");
      break;
  default:
    Serial.println("Invalid Command");
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

  // initialize headlight pin as output
  pinMode(HEADLIGHT, OUTPUT);

  // initialize stepper motor pins as outputs
  initStepperController();

  // TODO initialize servo motors
  resetPID();

  delay(5); // give some time for setup (5 milliseconds - shouldn't make a perceptible difference)

  pinMode(GLOBAL_ENABLE, OUTPUT);
  digitalWrite(GLOBAL_ENABLE, HIGH);


  pinMode(8, OUTPUT); 
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


  int interval = 1000; 
  int secondsToReset = 10000; 
  int millisecondsPassed = 0; 

  while (millisecondsPassed < secondsToReset) { 
    millisecondsPassed += interval; 
  }

  if (millisecondsPassed > secondsToReset) { 
    millisecondsPassed = 0; 
    disablePins(); 
  }

  while (Serial) { 
    warningLight(); 
    disablePins(); 

    Serial.println("SERIAL DISCONNECTED"); 
  }
}

void disablePins() { 
    CORE_PIN20_CONFIG = 0; 
    CORE_PIN19_CONFIG = 0; 
    CORE_PIN16_CONFIG = 0; 
    CORE_PIN15_CONFIG = 0; 
    CORE_PIN38_CONFIG = 0; 
    CORE_PIN37_CONFIG = 0; 
    CORE_PIN34_CONFIG = 0; 
    CORE_PIN33_CONFIG = 0; 

    Serial.println("DISABLE"); 
}


void warningLight() { 

  while (showWarning) { 
    digitalWrite(8, HIGH);  
    delay(1000);                      
    digitalWrite(8, LOW);   
    delay(1000);  
  }

    Serial.println("WARNING LIGHT"); 
}