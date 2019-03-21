/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_system.h"
#include "mgos_timers.h"

#include "rotary_encoder.h"

// Callbacks for set point changing
int16_t sp = 0;
inline void decrementSp(void)
{
	sp--;
	if (sp < 0)
		sp = 0;
	LOG(LL_INFO, ("target temperature changed to: %.2f", sp / 4.0f));
}

inline void incrementSp(void)
{
	sp++;
	if (sp > 600)
		sp = 600;
	LOG(LL_INFO, ("target temperature changed to: %.2f", sp / 4.0f));
}

// Rotary encoder and associated state machine
rotaryEncoder knob(18, 6, decrementSp, incrementSp);

// Enable refiring 
static void zeroCrossingReset(void *arg)
{
	mgos_gpio_enable_int(15);
	LOG(LL_INFO, ("new PWM cycle"));
}

uint16_t totalTime = 16;
uint16_t onTime = 16;

// Starts PWM cycle
static void zeroCrossingLineup(int pin, void *arg)
{
	// Disable interrupts for a 1 second interval and kick off PWM
	mgos_gpio_disable_int(15);
	mgos_gpio_blink(50, onTime, totalTime - onTime);
	(void) pin;
	(void) arg;
}

float reading = 0;

// Does some bit banging to get fixed point temperature data
static void getReading(void *arg) {

	(void) arg;
}

float iTerm = 0;
float lastInput = 0;

// Pid calculations to compute onTime
static void pid(void *arg) {
	float Kp = 0.25 / (float) totalTime;
	float Ki = 0.125 / (float) totalTime;
	float Kd = 0.0625 / (float) totalTime;

	float actualSp = sp / 4.0f;
	
	float err = sp - reading;

	float dErr = reading - lastInput;
	iTerm += Ki * error;

	lastInput = reading;

	float output = Kp * error + iTerm - Kd * dErr;

	if (output < 0.0f) output = 0.0f;
	if (output > (float) totalTime) output = (float) totalTime;

	onTime = (uint16_t) output;
	(void) arg;
}

enum mgos_app_init_result mgos_app_init(void)
{
	mgos_set_timer(16, MGOS_TIMER_REPEAT, zeroCrossingReset, NULL);
	mgos_set_timer(10, MGOS_TIMER_REPEAT, getReading, NULL);
	mgos_gpio_setup_input(15, MGOS_GPIO_PULL_UP);
	mgos_gpio_set_int_handler(15, MGOS_GPIO_INT_EDGE_NEG, zeroCrossingLineup, NULL);

	mgos_gpio_setup_output(50, false);

	return MGOS_APP_INIT_SUCCESS;
}
