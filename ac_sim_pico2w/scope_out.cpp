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
    gpio_init(SCOPE_PIN_1);
    gpio_set_dir(SCOPE_PIN_1, GPIO_OUT);
    gpio_put(SCOPE_PIN_1, false);
}


void ScopeOut::bin_out(bool hi) {
    gpio_put(SCOPE_PIN_1, hi);
}


void ScopeOut::toggle() {
    gpio_xor_mask(1u << SCOPE_PIN_1);
}
