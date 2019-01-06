#ifndef PID_H
#define PID_H

// Old: https://onlinegdb.com/HyecunzV2m
// New: https://onlinegdb.com/ryrjjgD2m
// Some good coefficients are:
// Kp = 0.6
// Ki = 0.5
// Kd = 0.25 ? 0
// Kd is not very good

#include "fixed_point.h"
#include "pwm.h"

// Struct to encapsulate the user variables
struct variables {
    fp32_t input;							// Stores input
    int32_t set_point;						// Stores set point
};

// Struct for PID timing information
struct timing {
    uint32_t sample_time;					// Stores frequency
    uint32_t time_stamp;					// Stores last call time
    uint32_t tuning_ticks;					// Stores progress to next self tune
    uint32_t tuning_period;					// Stores tuning frequency
};

// Struct to clamp the PID output
struct limits {
    int32_t out_min;						// Output clamping minimum
    int32_t out_max;						// Output clamping maximum
};

// Struct to encapsulate PID coefficients
struct coefficients {
    fp32_t Kp;								// Proportional constant in float that gets converted to fixed point
    fp32_t Ki;								// Integral constant in float that gets converted to fixed point
    fp32_t Kd;								// Derivative constant in float that gets converted to fixed point
};

// Struct to contain recursive information
struct state_variables {
    fp32_t last_input;						// Last input to the system in fixed point
    fp32_t i_term;							// Current error sum in fixed point
    fp32_t cum_err;							// Tuning error, absolute value of all errors added up
};

// The controller object
struct controller {
    struct variables vars;
    struct timing times;
    struct limits lims;
    struct coefficients coeffs;
    struct state_variables s_vars;
};

// User arguments to the controller
struct controller_args {
    uint32_t sample_time;
    uint32_t tuning_period;
    struct limits lims;
    struct coefficients coeffs;
};

struct controller *ctls;

// Controller constructor
void controller_init(struct controller *ctl, struct controller_args *args);

// Adjusts set point
void set_sp(struct controller *ctl, int32_t sp);

// Adjusts sample time in ms
void set_sample_time(struct controller_args *ctl_args, uint32_t st);

// Adjusts self tuning period
void set_tuning_period(struct controller_args *ctl_args, uint32_t tp);

// Adjusts clamping range
void set_limits(struct controller_args *ctl_args, int32_t out_min, int32_t out_max);

// Adjusts PID coefficients
void set_coefficients(struct controller_args *ctl_args, float Kp, float Ki, float Kd);

// Self tunes PID every once in a while
// WIP: EXTREMELY UNSTABLE
void self_tune(struct controller *ctl);

// Control algorithm
int32_t control(struct controller *ctl, fp32_t input);

#endif
