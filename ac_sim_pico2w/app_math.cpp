/* 
 * Project math functions
*/

#include "hardware/clocks.h"
#include "app_math.h"


SampRatePreCalc::SampRatePreCalc() {
    init_precalc(60.0);
}


SampRatePreCalc::SampRatePreCalc(float freq) {
    init_precalc(freq);
}


// Pre-calculate some values for the ISR 
void SampRatePreCalc::init_precalc(float freq) {
    this->freq = freq;
    wrap_val = SYS_CLK_HZ / ((2*360)*freq) - 1; 
    pwm_50pct_level = 0.5f * wrap_val;
    samps_per_deg = 2;
    deg_per_samp = 1.0f / (float) samps_per_deg;
    samps_per_cycle = 360 * samps_per_deg;  // 360 deg * 2 samp/deg 
    steps_to_rads = F_2PI / samps_per_cycle;
    rads_to_steps = 1.0f / steps_to_rads;

    // set up sin table
    sin_table = new float(samps_per_cycle);
    for (int i = 0; i < samps_per_cycle; ++i) {
        sin_table[i] = sinf(steps_to_rads * (float) i);
    }
}


/* faster sine funct
    TBD; lookup table
    angle from 0 to 2pi
*/
float SampRatePreCalc::fast_sin(float angle) {
    return sinf(angle);  //TODO: remove this and fix this func (phase issue)

    float result;

    // result = sinf(angle);
    //angle = fmodf(angle + F_2PI, F_2PI);  // normalize > 0 then < 2pi
    uint16_t index = (uint16_t) (rads_to_steps * angle );
    result = sin_table[index];

    return result;
}

