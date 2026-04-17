#ifndef SCOPE_OUT_H
#define SCOPE_OUT_H

#include "hardware/gpio.h"

#include "app_io_pins.h"


class ScopeOut {
public: 
    ScopeOut();

    void setup();
    void bin_out(bool hi);
    void toggle();
}; 

#endif // SCOPE_OUT_H 
