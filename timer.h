#ifndef TIMER_H
#define TIMER_H

#include <ti/drivers/Timer.h>
#include "Board.h"
#include "c_imports.h"
#include "gpio.h"

// Sets up a timer for use
void init_timer();

// Interrupt for zero crossing
void zero_crossing_reset(Timer_Handle handle);
#endif
