#ifndef FILTER_H
#define FILTER_H

#include "fixed_point.h"

// Filter modes of operation
#define WINDOW 0
#define EXPONENTIAL 1

// Defines a moving average filter
struct moving_average {
    uint8_t taps;                                           // Number of taps
    uint8_t curr_idx;                                       // Circular buffer index
    fp32_t *buffer;                                         // List of samples
    fp32_t curr_avg;                                        // Fixed point representation of current average
};

// Defines an exponential moving average filter
struct exp_average {
    fp32_t alpha;                                           // Alpha parameter for EMA
    fp32_t curr_avg;                                        // Current average in fixed point
};

// Filter function typedefs
typedef void (*filter_func)(void *, fp32_t);
typedef void (*mv_avg)(struct moving_average *, fp32_t);
typedef void (*ema_avg)(struct exp_average *, fp32_t);

// Sliding window filter

// Constructor
void init_avg_filter(struct moving_average *self, uint8_t taps);

// Update function
void avg_filter(struct moving_average *self, fp32_t input);

// Get() function
int32_t get_mv_avg(struct moving_average *self);

// Destructor
void destroy_moving_avg(struct moving_average *self);

// Exponential moving average filter

// Constructor
void init_ema_filter(struct exp_average *self, float alpha);

// Update function
void ema_filter(struct exp_average *self, fp32_t input);

// Get() function
int32_t get_ema_avg(struct exp_average *self);

// Destructor
void destroy_exp_avg(struct exp_average *self);

#endif
