#include "rotary_encoder.h"

// Define the state table that the machine uses
const int8_t rotaryEncoder::stateMachine[16] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0
};

rotaryEncoder::rotaryEncoder(const uint8_t chA, const uint8_t chB, encdrCallback ccwCallback, encdrCallback cwCallback) {
    // Set the instance's channel to the passed-in parameters
    // Also associate the callbacks with the instance
    this->chA = chA;
    this->chB = chB;
    this->ccwCallback = ccwCallback;
    this->cwCallback = cwCallback;

    this->stateIdx = 3;
    this->count = 0;

    // Set each channel pin as an input and enable an interrupt handler for each, triggered on both edges
    // Pass in the instance so that the IRQ has the ability to operate on the class member
    mgos_gpio_setup_input(chA, MGOS_GPIO_PULL_DOWN);
    mgos_gpio_set_int_handler(chA, MGOS_GPIO_INT_EDGE_ANY, rotaryEncoder::transition, this);
    mgos_gpio_setup_input(chB, MGOS_GPIO_PULL_DOWN);
    mgos_gpio_set_int_handler(chB, MGOS_GPIO_INT_EDGE_ANY, rotaryEncoder::transition, this);

    // Enable pin interrupts
    mgos_gpio_enable_int(chA);
    mgos_gpio_enable_int(chB);
}

// Transition function for rotary encoder
void rotaryEncoder::transition(int pin, void *arg) {
    // Take the passed in argument and reinterpret it as a rotaryEncoder
    rotaryEncoder *encdr = reinterpret_cast<rotaryEncoder *>(arg);

    // Read the pins
    const uint8_t aVal = mgos_gpio_read(encdr->chA);
    const uint8_t bVal = mgos_gpio_read(encdr->chB);

    // Encode the pin values in binary
    const uint8_t state = (aVal << 1) | bVal;

    // Push the state to the queue
    encdr->stateIdx <<= 2;
    encdr->stateIdx |= state;

    // Update the rotary encoder counter
    encdr->count += rotaryEncoder::stateMachine[encdr->stateIdx & 15];
    
    // If enough counterclockwise ticks have occurred, perform the output
    if (encdr->count < -3) {
        encdr->count = 0;
        encdr->ccwCallback();
    }

    // If enough clockwise ticks have occurred, perform the output
    if (encdr->count > 3) {
        encdr->count = 0;
        encdr->cwCallback();
    }
}
