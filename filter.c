/*
 * filter.c
 *
 *  Created on: Oct 30, 2018
 *      Author: krad2
 */

#include "filter.h"

// Constructor
void init_avg_filter(struct moving_average *self, uint8_t taps) {
    // Sets the tap count
    self->taps = taps;

    // Starts the queue at the beginning
    self->curr_idx = 0;

    // Initializes an array of variable size fitted to the number of taps
    self->buffer = (fp32_t *) calloc(taps, sizeof(fp32_t));

    // Running average
    self->curr_avg = 0;
}

// Simple moving average function, returns answer as a fixed point number
void avg_filter(struct moving_average *self, fp32_t input) {
    // Extracts the size of the buffer
    uint8_t taps = self->taps;

    // Extracts current location in buffer
    uint8_t iter = self->curr_idx;

    // Extracts buffer of samples
    fp32_t *buffer = self->buffer;

    // Average to be updated
    fp32_t old_avg = self->curr_avg;

    // Compute y[n] = I + y[n - 1] - x[n]
    fp32_t new_avg = input + old_avg - buffer[iter];

    // Store the new sample at the location
    buffer[iter] = input;

    // Move the iterator forward
    self->curr_idx = ++iter % taps;

    // Update the moving average
    self->curr_avg = new_avg;
}

// Get() function
int32_t get_mv_avg(struct moving_average *self) {
    // Gets the current average, converts it from fixed point to normal
    // Divides it by the number of taps because update() only totals a running sum
    return FP32_UNSCALE(self->curr_avg / self->taps);
}

// Destroys the average filter
void destroy_moving_avg(struct moving_average *self) {
    // Deletes internal buffer and destroys object
    free(self->buffer);
    free(self);
}

// Constructor
void init_ema_filter(struct exp_average *self, float alpha) {
    // Takes the alpha from float type to fixed point type
    self->alpha = (fp32_t) FP32_SCALE(alpha);

    // Running average
    self->curr_avg = 0;
}

// Exponential moving average function, returns answer as a fixed point number
void ema_filter(struct exp_average *self, fp32_t input) {
    // Extracts the alpha coefficient in fixed point
    fp32_t alpha = self->alpha;

    // Extracts 1 - alpha in fixed point
    fp32_t one_minus_alpha = FP32_SCALE(1) - alpha;

    // Depending on the mode, keep or drop precision
    // Compute y[n] = a * x[n] + (1 - a) * y[n - 1]
    fp32_t new_avg = fp32_mul(alpha, input) + fp32_mul(one_minus_alpha, self->curr_avg);

    // Updates average in fixed point
    self->curr_avg = new_avg;
}

// Get() function
int32_t get_ema_avg(struct exp_average *self) {
    // Takes fixed point average and converts to normal
    return FP32_UNSCALE(self->curr_avg);
}

// Destructor
void destroy_exp_avg(struct exp_average *self) {
    free(self);
}
