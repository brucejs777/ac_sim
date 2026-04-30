ac_sim_pico2w

 * Copyright (c) 2026 Bruce J Snyder
 * License: MIT
 *
ver 0.5; 2026.04.30: Changed some defines and consts
ver 0.4; 2026.04.17: Convert to C++
ver 0.3; 2026.04.10: Expand to three sine waves with phase and amplitude. 
    Add chan/level/phase in-place printout using ANSI - must set terminal to ANSI.
    Add scope output to track main loop cycles; high when busy.
    Add struct to hold PWM settings.
ver 0.2; 2026.04.02: Create 60 Hz sine wave using PWM on GPIO 7 & 8.
ver 0.1; 2026.03.19: Initial version in C.  
    Use RP2350 PWM block & ISR to create one 60Hz sine wave on GPIO 0.  
    Add another on GPIO 1.
