/* *************************************************************
   Encoder definitions

   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.

   ************************************************************ */

#include "Arduino.h"
#include "commands.h"
#include "pinout.h"
#include "encoder_driver.h"

// Not sure how this code works

volatile long left_enc_pos = 0L;
volatile long right_enc_pos = 0L;

// based on the valid states inside an encoder. NOTE: Will most likely need to be changed. each state is a 4 bit number
// (16 combinations) and corresponds to a direction
static const int8_t ENC_STATES[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};

/* Interrupt routine for encoder, taking care of actual counting */
void ISR(int addr)
{
  static uint8_t enc_last = 0;

  enc_last <<= 2;                     // shift previous state two places
  enc_last |= (PIND & (3 << 2)) >> 2; // read the current state into lowest 2 bits

  left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
}

/* Wrap the encoder reading function */
long readEncoder(int i)
{
  if (i == LEFT)
    return left_enc_pos;
  else
    return right_enc_pos;
}

/* Wrap the encoder reset function */
void resetEncoder(int i)
{
  if (i == LEFT)
  {
    left_enc_pos = 0L;
    return;
  }
  else
  {
    right_enc_pos = 0L;
    return;
  }
}

/* Wrap the encoder reset function */
void resetEncoders()
{
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

inline void WRIST_INCLINATION_ISR()
{
  ISR(WRIST_INCLINATION);
}

inline void WRIST_ROTATION_ISR()
{
  ISR(WRIST_ROTATION);
}

inline void GRIPPER_ISR()
{
  ISR(GRIPPER);
}

inline void BASEMOTOR_ISR()
{
  ISR(BASEMOTOR);
}
