#ifndef ROTARY_ENC_H
#define ROTARY_ENC_H
#include "Board.h"
#include <ti/drivers/GPIO.h>
#include <stdint.h>
#include <stdio.h>
#define ENCODER_A 1
#define ENCODER_B 2
#define ENCODER_BUTTON 3

// Referencing this:
// http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html?m=1

// Encoder state is in the format A B
// Collective state is a 4 bit number (old)(new)
// Most significant 2 bits are the old state of the encoder
// Least significant 2 bits are the new state of the encoder
// For instance, 0000 = (00)(00)
// Thus neither A nor B has changed

// Lookup table for encoder state machine
// Represents directions as a function of state
static int8_t encoder_lut[16] = {
      0,    // 00 -> 00 = None
     -1,    // 00 -> 01 = CCW
      1,    // 00 -> 10 = CW
      0,    // 00 -> 11 = None
      1,    // 01 -> 00 = CW
      0,    // 01 -> 01 = None
      0,    // 01 -> 10 = None
     -1,    // 01 -> 11 = CCW
     -1,    // 10 -> 00 = CCW
      0,    // 10 -> 01 = None
      0,    // 10 -> 10 = None
      1,    // 10 -> 11 = CW
      0,    // 11 -> 00 = None
      1,    // 11 -> 01 = CW
     -1,    // 11 -> 10 = CCW
      0     // 11 -> 11 = None
};

void encoder_transition(uint_least8_t index);
void Aint();

void Bint();
#endif
