/*
 * Misc math items for the project

 * Notes: 
 * added _USE_MATH_DEFINES in CMakeLists.txt for M_PI etc
*/

#pragma once

#include <math.h>
#include <cstdint>


// set top to get pwm period (default 2^16 - 1)
// set for 360deg * 2 steps per degree * 60 Hz = 43200 sample rate -> 3472.222 Hz
// 150MHz / (3472.0 * 2 * 360) = 60.003840 Hz ;]
constexpr float F_2PI = 2.0f * (float) M_PI;
constexpr float f_deg_to_rad = F_2PI / 360.0f;
constexpr float f_rad_to_deg = 360.0f / F_2PI;


struct SampRatePreCalc {
    // struct to hold pre-calculated values after knowing sample rate
    // purpose: releive the ISR from repeatedly doing these calcs; clarify what values are needed
    // TODO: re-arrange such that the sample rate is given instead
    float freq;
    uint16_t wrap_val;
    float pwm_50pct_level;
    short samps_per_deg;
    float deg_per_samp;
    short samps_per_cycle;
    float steps_to_rads;
    float rads_to_steps;

    float *sin_table = NULL;

    SampRatePreCalc();
    SampRatePreCalc(float freq);

    void init_precalc(float freq);

    float fast_sin(float angle);
};
