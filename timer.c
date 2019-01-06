#include "timer.h"

void init_timer() {
    /* Period and duty in microseconds */
    Timer_Handle timer0;
    Timer_Params params;
    Timer_init();

    /* Setting up the timer in continuous callback mode that calls the callback
     * function every 1,000,000 microseconds, or 1 second.
     */

    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = zero_crossing_reset;

    timer0 = Timer_open(Board_TIMER0, &params);
    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1);
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1);
    }
}

// Every time the timer overflows, the zero crossing resets
void zero_crossing_reset(Timer_Handle myHandle)
{
    GPIO_enableInt(ZERO_CROSS);
}
