#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include "c_imports.h"

// Defines a 32 bit fixed point type
typedef int32_t fp32_t;

// fp32_t has N fractional bits and 32 - N integral bits
#define FRACT32_BITS 7

// fp32_t has an internal scale factor
#define SCALE32_FACTOR (1 << FRACT32_BITS)

// To convert a number to fixed point, multiply by the scale factor
#define FP32_SCALE(x) ((x) * SCALE32_FACTOR)

// To map that to a float, divide by the scale factor
#define FP32_UNSCALE(x) ((x) / SCALE32_FACTOR)

fp32_t fp32_mul(fp32_t x, fp32_t y);
fp32_t fp32_div(fp32_t x, fp32_t y);

#endif
