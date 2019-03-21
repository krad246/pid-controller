#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "mgos_gpio.h"

// Encoder callback function
typedef void (*encdrCallback)(void);

// Rotary encoder class that hooks into MGOS
class rotaryEncoder {
public:
    // Constructor which takes 2 channels and a callback for each direction
    rotaryEncoder(const uint8_t chA, const uint8_t chB, encdrCallback ccwCallback, encdrCallback cwCallback);

private:
    // Pin channels
    uint8_t chA;
    uint8_t chB;

    // State machine variables
    uint8_t stateIdx;
    int8_t count;

    // Callbacks for the instance of the encoder
    encdrCallback ccwCallback;
    encdrCallback cwCallback;

    // Shared transition function that hooks into the kernel
    static void transition(int pin, void *arg);

    // State table that all instances of the class use
    static const int8_t stateMachine[16];
};

#endif
