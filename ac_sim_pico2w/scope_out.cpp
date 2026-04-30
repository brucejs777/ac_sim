/* 
 * scope_out.cpp
 *
 * Simple class of functions to put out debug states to gpio pins
 * 
 */

 #include "scope_out.h"


ScopeOut::ScopeOut() {
    setup();
}


void ScopeOut::setup() {
    gpio_init(kScopePin_1);
    gpio_set_dir(kScopePin_1, GPIO_OUT);
    gpio_put(kScopePin_1, false);
}


void ScopeOut::bin_out(bool hi) {
    gpio_put(kScopePin_1, hi);
}


void ScopeOut::toggle() {
    gpio_xor_mask(1u << kScopePin_1);
}
