/*
 * gpio.h
 *
 *  Created on: Oct 31, 2018
 *      Author: krad2
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <ti/drivers/GPIO.h>
#include "Board.h"

#include "pwm.h"
#include "pid.h"

// Indices of pins in use
#define ZERO_CROSS 0
#define CHIP_ENABLE 5

// Sets up GPIO pins
void init_gpio();

// Callbacks to start / stop the PWM timers correctly
extern void zero_crossing_lineup(uint_least8_t index);

#endif /* GPIO_H_ */
