/*
 * gpio.c
 *
 *  Created on: Oct 31, 2018
 *      Author: krad2
 */
#include "gpio.h"
#include "Rotary Encoder.h"

// Sets up GPIO pins
void init_gpio() {
	GPIO_init();

	// Enables zero crossing callbacks on zero crossing pin
	GPIO_enableInt(ZERO_CROSS);
	GPIO_setCallback(ZERO_CROSS, zero_crossing_lineup);
	//initialize rotary encoder info
    GPIO_enableInt(ENCODER_A);
    GPIO_enableInt(ENCODER_B);

    GPIO_setCallback(ENCODER_A, encoder_transition);
    GPIO_setCallback(ENCODER_B, encoder_transition);
}
