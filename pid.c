#include "pid.h"

// Constructs a controller object
void controller_init(struct controller *ctl, struct controller_args *args) {
    // Allocates member variables
    struct variables vars;
    struct timing times;
    struct limits lims;
    struct coefficients coeffs;
    struct state_variables s_vars;

    if (args) {
		// Assigns initial conditions and parameters
		vars = (struct variables) {
			.input = 0,
			.set_point = 0
		};

		times = (struct timing) {
			.sample_time = args->sample_time,
			.time_stamp = 0,
			.tuning_ticks = 0,
			.tuning_period = args->tuning_period
		};

		lims = args->lims;
		coeffs = args->coeffs;

		s_vars = (struct state_variables) {
			.last_input = 0,
			.i_term = 0,
			.cum_err = 0
		};
    }

    // Sets the controller up with members
    ctl->vars = vars;
    ctl->times = times;
    ctl->lims = lims;
    ctl->coeffs = coeffs;
    ctl->s_vars = s_vars;
}

// Sets set point
void set_sp(struct controller *ctl, int32_t sp) {
	// Convert to set point for internal use
    ctl->vars.set_point = sp;
}

// Sets sample frequency
void set_sample_time(struct controller_args *ctl_args, uint32_t st) {
    // Updates sample time to desired
    ctl_args->sample_time = st;
}

// Sets self tune frequency
void set_tuning_period(struct controller_args *ctl_args, uint32_t tp) {
    ctl_args->tuning_period = tp;
}

// Sets limits of PID output
void set_limits(struct controller_args *ctl_args, int32_t out_min, int32_t out_max) {
    struct limits lims;
    lims.out_min = out_min;
    lims.out_max = out_max;

    ctl_args->lims = lims;
}

// Sets coefficients of controller
void set_coefficients(struct controller_args *ctl_args, float Kp, float Ki, float Kd) {
    // Converts coefficients from floating point to fixed point
    struct coefficients fp_coefficients = (struct coefficients) {
        .Kp = (fp32_t) FP32_SCALE(Kp),
        .Ki = (fp32_t) FP32_SCALE(Ki),
        .Kd = (fp32_t) FP32_SCALE(Kd)
    };

    // Updates controller with new values
    ctl_args->coeffs = fp_coefficients;
}

// Self tuning for PID
// WIP: EXTREMELY UNSTABLE
// CAUSES LOSS OF CONVERGENCE
/*
void self_tune(struct controller *ctl) {
    // Extract all state information
    struct variables vars = ctl->vars;
    struct timing times = ctl->times;
    struct coefficients coeffs = ctl->coeffs;
    struct state_variables s_vars = ctl->s_vars;

    double sample_time = (double) ctl->times.sample_time;

    // Calculate coefficient correction
    double correction = s_vars.cum_err / (times.tuning_period * vars.set_point);

    // Apply correction
    struct coefficients new_coeffs = (struct coefficients) {
        .Kp = coeffs.Kp - correction,
        .Ki = coeffs.Ki - correction * sample_time,
        .Kd = coeffs.Kd - correction / sample_time
    };

    // Reset the loop and push the changes to the controller
    ctl->times.tuning_ticks = 0;
    ctl->s_vars.cum_err = 0;
    set_coefficients(ctl, new_coeffs);
}
*/
// PID control algorithm
int32_t control(struct controller *ctl, fp32_t input) {
	// Plug in the input
	ctl->vars.input = input;

    // Extract all state information
    struct variables vars = ctl->vars;
    struct timing times = ctl->times;
    struct limits lims = ctl->lims;
    struct coefficients coeffs = ctl->coeffs;
    struct state_variables s_vars = ctl->s_vars;

    // Get the current time from the RTC
    struct timespec curr_time;
    clock_gettime(CLOCK_REALTIME, &curr_time);

    // Convert to milliseconds
    uint32_t ms_time = (curr_time.tv_sec * 1000) + (curr_time.tv_nsec / 1000000);

    // Compute the elapsed time since the last call
    uint32_t now = ms_time;
    uint32_t last = times.time_stamp;
    uint32_t dt = now - last;

    // If it's time to adjust
    if (dt >= times.sample_time) {
        // Extract the bounds for the output and integral term
        fp32_t out_min = FP32_SCALE(lims.out_min);
        fp32_t out_max = FP32_SCALE(lims.out_max);

        // Extract the coefficients
        fp32_t Kp = coeffs.Kp;
        fp32_t Ki = coeffs.Ki;
        fp32_t Kd = coeffs.Kd;

        // Extract the error
        fp32_t error = FP32_SCALE(vars.set_point) - vars.input;

        // Compute and clamp the integral term
        fp32_t i_term = s_vars.i_term + fp32_mul(Ki, error);

        if (i_term < out_min) i_term = out_min;
        if (i_term > out_max) i_term = out_max;

        // Extract the derivative of the error
        fp32_t d_err = vars.input - s_vars.last_input;

        // Compute and clamp the PID output
        fp32_t out = fp32_mul(Kp, error) + i_term - fp32_mul(Kd, d_err);
        if (out < out_min) out = out_min;
        if (out > out_max) out = out_max;

        // Adjust time stamps
        ctl->times.tuning_ticks++;
        ctl->times.time_stamp = now;

        // Save recurrence information
        ctl->s_vars.last_input = vars.input;
        ctl->s_vars.i_term = i_term;

        // If it's time to self tune
        if (ctl->times.tuning_ticks >= times.tuning_period) {
            // self_tune(ctl);
        } else {
        	/**
        	 *  // Prepare for self tuning
        	 *  int32_t err_mask = error >> 31;
             *  ctl->s_vars.cum_err += FP32_SCALE((err_mask ^ error) - err_mask);
             */
        }

        // Return the PID output
        return FP32_UNSCALE(out);
    }

    // Else return a 'dummy' value
    return lims.out_min - 1;
}
