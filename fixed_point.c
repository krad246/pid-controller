/*
 * fixed_point.c
 *
 *  Created on: Oct 30, 2018
 *      Author: krad2
 */

#include "fixed_point.h"

/*
 * Multiplies 2 fixed point numbers
 * x and y are fixed point types (includes scale factor s)
 * Product of 2 fixed point numbers s * x, s * y is s * s * x * y
 * Returns the answer as a fixed point number by unscaling once
 */
fp32_t fp32_mul(fp32_t x, fp32_t y) {
    // Unscales result of computation once to yield fp32_t
    return FP32_UNSCALE(
        // Extends number width to prevent loss of precision
        (int64_t) x * (int64_t) y
    );
}

/*
 * Divides 2 fixed point numbers
 * x and y are fixed point types (includes scale factor s)
 * Division of 2 fixed point numbers s * x, s * y is x / y
 * Returns the answer as a fixed point number by scaling once
 */
fp32_t fp32_div(fp32_t x, fp32_t y) {
    // Scales result of computation to yield fp32_t
    return FP32_SCALE(
        // Extends number width to prevent loss of precision
        (int64_t) x
    ) / y;
}
