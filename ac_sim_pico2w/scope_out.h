/* scope_out.h */

#pragma once

#include "hardware/gpio.h"

#include "app_io_pins.h"


class ScopeOut {
public: 
    ScopeOut();

    void setup();
    void bin_out(bool hi);
    void toggle();
}; 
