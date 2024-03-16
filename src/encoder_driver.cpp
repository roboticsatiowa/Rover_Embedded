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

volatile long left_enc_pos = 0L;
volatile long right_enc_pos = 0L;

const uint BASEMOTOR = 0;
const uint WRIST_INCLINATION = 1;
const uint WRIST_ROTATION = 2;
const uint GRIPPER = 3;

// based on the valid states inside an encoder. each state is a 4 bit number (2 bits previous state + 2 bits current state)
// (16 combinations) and corresponds to a direction
static const int8_t ENC_STATES[] = { 0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0 };

volatile long encoder_angles[] = { 0L, 0L, 0L, 0L };

static uint8_t enc_last[] = { 0, 0, 0, 0 };
/* Interrupt routine for encoder, taking care of actual counting */
void ISR(int addr) {
  // find the corresponding encoder pins
  int encoder_pin_a, encoder_pin_b = -1;
  switch (addr) {
  case BASEMOTOR:
    encoder_pin_a = BASEMOTOR_ENC_A;
    encoder_pin_b = BASEMOTOR_ENC_B;
    break;
  case WRIST_INCLINATION:
    encoder_pin_a = WRIST_INCLINATION_ENC_A;
    encoder_pin_b = WRIST_INCLINATION_ENC_B;
    break;
  case WRIST_ROTATION:
    encoder_pin_a = WRIST_ROTATION_ENC_A;
    encoder_pin_b = WRIST_ROTATION_ENC_B;
    break;
  case GRIPPER:
    encoder_pin_a = GRIPPER_ENC_A;
    encoder_pin_b = GRIPPER_ENC_B;
    break;
  default:
    break;
  }

  enc_last[addr] <<= 2;                                                                           // shift previous state two places
  enc_last[addr] = enc_last[addr] | digitalRead(encoder_pin_b) << 1 | digitalRead(encoder_pin_a); // read the current state into lowest 2 bits

  encoder_angles[addr] += (long)ENC_STATES[(enc_last[addr] & 0b1111)];
}

/* Wrap the encoder reading function */
long readEncoder(int addr) {
  return encoder_angles[addr];
}

/* Wrap the encoder reset function */
void resetEncoder(int i) {
  encoder_angles[i] = 0L;
}

/* Wrap the encoder reset function */
void resetAllEncoders() {
  for (int i = 0; i < 4; i++) {
    resetEncoder(i);
  }
}

void WRIST_INCLINATION_ISR() {
  ISR(WRIST_INCLINATION);
}

void WRIST_ROTATION_ISR() {
  ISR(WRIST_ROTATION);
}

void GRIPPER_ISR() {
  ISR(GRIPPER);
}

void BASEMOTOR_ISR() {
  ISR(BASEMOTOR);
}
