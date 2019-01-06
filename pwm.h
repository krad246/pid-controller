/*
 * pwm.h
 *
 *  Created on: Oct 25, 2018
 *      Author: Jime
 */

#ifndef PWM_H_
#define PWM_H_

#include <ti/drivers/PWM.h>
#include "Board.h"

#include "c_imports.h"

#define VAC_PERIOD 120                                      // Number of half cycles in 1 second at 60 Hz
#define HALF_CYCLE_PERIOD 1                                 // The delay of a single cycle wrt the max period

#define STEP_SIZE PWM_DUTY_FRACTION_MAX / VAC_PERIOD        // HALF_CYCLE_PERIOD in timer coounts

#define VAC_COUNT VAC_PERIOD * STEP_SIZE                    // Size of a total period in increments
#define HALF_CYCLE_COUNT HALF_CYCLE_PERIOD * STEP_SIZE      // Size of a half cycle in increments

#define CYCLE_RES 15                                        // Number of PWM timer divisions in 1 second
#define MAX_COUNT CYCLE_RES                                 // Same thing, just a different name
#define PID_RANGE VAC_PERIOD / CYCLE_RES                    // Maximum number of cycles in 1 PWM timer interval
#define EFFECTIVE_FREQ CYCLE_RES                            // Number of PWM timer interrupts per second in Hz

PWM_Handle switchers[2];

// Creates the switcher PWM instance
void init_pwm();

// Resets pwm objects
void reset_pwm(unsigned int duty_PWM0, unsigned int duty_PWM1);

#endif /* PWM_H_ */
