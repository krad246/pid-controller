/*
 * pwm.c
 *
 *  Created on: Oct 24, 2018
 *      Author: krad2
 */
#include "pwm.h"

void init_pwm() {
	// Initialize the driver
	PWM_init();

	PWM_Params params;
	PWM_Params_init(&params);

	// Creates the timer so that 1 period = 1 60 Hz period
	params.periodUnits = PWM_PERIOD_HZ;
	params.periodValue = EFFECTIVE_FREQ;

	// Starts at 0% duty cycle
	params.dutyUnits = PWM_DUTY_FRACTION;
	params.dutyValue = 0;

	// Sets up, but does not start the PWM instance
	PWM_Handle pwm_0 = PWM_open(Board_PWM0, &params);
	PWM_Handle pwm_1 = PWM_open(Board_PWM1, &params);

	if (pwm_0 == NULL) {
		/* Board_PWM did not open */
		printf("Error initializing PWM device.\n");
		while (1);
	}

    if (pwm_1 == NULL) {
        /* Board_PWM did not open */
        printf("Error initializing PWM device.\n");
        while (1);
    }

	switchers[0] = pwm_0;
	switchers[1] = pwm_1;
}

// Resets PWM objects
void reset_pwm(unsigned int duty_PWM0, unsigned int duty_PWM1){
    // Shuts off device
    PWM_close(switchers[0]);
    PWM_close(switchers[1]);

    // Reinitializes with same parameters as before
    PWM_Params params;
    PWM_Params_init(&params);

    // Creates the timer so that 1 period = 1 60 Hz period
    params.periodUnits = PWM_PERIOD_HZ;
    params.periodValue = EFFECTIVE_FREQ;

    // Starts at 0% duty cycle
    params.dutyUnits = PWM_DUTY_FRACTION;
    params.dutyValue = 0;

    // Sets up, but does not start the PWM instance
    PWM_Handle pwm_0 = PWM_open(Board_PWM0, &params);
    PWM_Handle pwm_1 = PWM_open(Board_PWM1, &params);

    if (pwm_0 == NULL) {
        /* Board_PWM did not open */
        printf("Error initializing PWM device.\n");
        while (1);
    }

    if (pwm_1 == NULL) {
        /* Board_PWM did not open */
        printf("Error initializing PWM device.\n");
        while (1);
    }

    switchers[0] = pwm_0;
    switchers[1] = pwm_1;

    // Sets duty to input
    PWM_setDuty(switchers[0], duty_PWM0);
    PWM_setDuty(switchers[1], duty_PWM1);
}
