/**
 * Copyright (c) 2026 Bruce J Snyder
 * License: MIT
 *
 * Notes: 
 * added _USE_MATH_DEFINES in CMakeLists.txt for M_PI etc
 * 
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
//#include "cmcis_gcc.h"
#include <math.h>

#include "app_io_pins.h"
#include "scope_out.h"
#include "app_math.h" 


ScopeOut scope;

static short g_phi = 0;  // main PWM index for sine wave group 

SampRatePreCalc srpc;


// TODO: make container struct for commonalities
// TODO: maybe use pwm config struct 
typedef struct PwmChanInfo {
    float amplitude;
    float phase;  // rads
    uint io_pin;
    uint chan;
    uint slice;  // 
    uint16_t wrap;
    float clkdiv;
} PwmChanInfo;  // pwm channel info 


#define NUM_PWM_OUTPUTS 3
PwmChanInfo g_pci[NUM_PWM_OUTPUTS];


void init_wave_params(PwmChanInfo pci[]) {
    /* Note: these pwm pins are paired into 'slices' dictated by hardware
    Each slice share a common counter & therefor a freq 
    Note: chan is 0 or 1, which half of the slice */
    uint pin;

    pin = 0;
    pci[pin].amplitude = 0.7f * 0.5 * (float) srpc.wrap_val;
    pci[pin].phase = 0.0;
    pci[pin].io_pin = APP_PWM_PIN_0;
    pci[pin].chan = pwm_gpio_to_channel(pci[pin].io_pin);
    pci[pin].slice = pwm_gpio_to_slice_num(pci[pin].io_pin);
    pci[pin].wrap = srpc.wrap_val;
    pci[pin].clkdiv = 1.0;
    pin = 1;
    pci[pin].amplitude = 0.5f * 0.5 *  (float) srpc.wrap_val;
    pci[pin].phase = f_deg_to_rad * 120.0f;
    pci[pin].io_pin = APP_PWM_PIN_1;
    pci[pin].chan = pwm_gpio_to_channel(pci[pin].io_pin);
    pci[pin].slice = pwm_gpio_to_slice_num(pci[pin].io_pin);
    pci[pin].wrap = srpc.wrap_val;
    pci[pin].clkdiv = 1.0;
    pin = 2;
    pci[pin].amplitude = 0.3f * 0.5 *  (float) srpc.wrap_val;
    pci[pin].phase = f_deg_to_rad * 240.0f;
    pci[pin].io_pin = APP_PWM_PIN_2;
    pci[pin].chan = pwm_gpio_to_channel(pci[pin].io_pin);
    pci[pin].slice = pwm_gpio_to_slice_num(pci[pin].io_pin);
    pci[pin].wrap = srpc.wrap_val;
    pci[pin].clkdiv = 1.0;
}


/* ISR to handle all 6 PWM counter re-loads - all share one pwm wrap irq */
void on_pwm_wrap() {
    float level;
    uint slice_num, channel_num;

    // Clear the interrupt flag that brought us here
    pwm_clear_irq(g_pci[0].slice);

    // calc current angle = time = phase;  may not be power of 2 and we don't like to / or %....
    if (++g_phi >= srpc.samps_per_cycle) {
        g_phi = 0;
    }

    // set all the levels of all the channels of all the slices at once
    for (uint pin = 0; pin < NUM_PWM_OUTPUTS; ++pin) {
        // calc new function value f0(g_phi) 
        level = srpc.pwm_50pct_level + 
            g_pci[pin].amplitude * srpc.fast_sin((srpc.steps_to_rads * (float) g_phi) + 
            g_pci[pin].phase );
        pwm_set_chan_level(g_pci[pin].slice, g_pci[pin].chan, level);
    }
}


/*  config a pwm pin
    all PWM slices share IRQ but each has a flag 
    DIS-ables IRQ and clears IRQ flag - enable IRQ AFTER this func */
void config_pwm_chan(PwmChanInfo pci) {
    pwm_clear_irq(pci.slice);
    //pwm_set_irq_enabled(pci.slice, true);

    gpio_set_function(pci.io_pin, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, pci.wrap);
    pwm_config_set_clkdiv(&config, pci.clkdiv);
    //pwm_set_counter(pci.slice, 0);  // pwm_init does this
    pwm_init(pci.slice, &config, true);
    // TODO: use pwm_set_mask_enabled for simultaneous ?

    pwm_set_chan_level(pci.slice, pci.chan, pci.wrap >> 1); // set to Vdd/2 = 1.65 VDC
}


/*  setup GPIO as PWM for six channels for app.  
    sine of same period on all 6.
    phase, amplitude can be different per channel.

    set some gpio pins to pwm
    Mask our slice's IRQ output into the PWM block's single interrupt line,
    and register our interrupt handler
    Note: USB stdio may have been interfering with the PWM
    try to set PWM higher priority; result: didn't help; TODO: what then ?
    set pwm period count
    Set divider, reduces counter clock to sysclock/this value
    Load the configuration into our PWM slice, and set it running.
    set the DC pin to the dc mid level for 50% square wave, for scope ref
*/
void config_app_pwm(PwmChanInfo pci[]) {
    uint en_mask = 0u;
    for (uint pin = 0; pin < NUM_PWM_OUTPUTS; ++pin) {
        config_pwm_chan(pci[pin]);
        en_mask |= 1u << pci[pin].slice;
    }

    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), on_pwm_wrap);
    // TODO: find out why USB stdio interferes with PWM; this priority setting did not help
    // default pri for all is 0x80 out of 0xFF 
    uint32_t usb_priority = irq_get_priority(USBCTRL_IRQ);
    irq_set_priority(PWM_IRQ_WRAP, (usb_priority - 1) & 0xFF);
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);
    pwm_set_mask_enabled(en_mask);
    pwm_set_irq_enabled(g_pci[0].slice, true);
}


/* simple console spinner to let user know we're not stuck 
put serial monitor of vsc into terminal mode for the \r to work */
void print_spinner() {
    static int spin_idx = 0;
    static char spin_chars[] = "|/-\\";
    uint num_chars = sizeof(spin_chars);

    printf("\r%c", spin_chars[spin_idx]);
    fflush(stdout);
    if (++spin_idx > num_chars)
        spin_idx = 0;
}


void print_channel_info() {
    // first time print blanks to make room elsewhere
    printf("\033[3A"); fflush(stdout);  // go up 3 lines
    for (uint chan = 0; chan < NUM_PWM_OUTPUTS; ++chan) {
        printf("\r\033[2K");  // erase line
        printf("CHAN=%2d: \tLevel = %6.1f; \tPhase = %5.1f\n", 
            chan, g_pci[chan].amplitude, f_rad_to_deg * g_pci[chan].phase);
    }
}


// main loop
int main() {
    uint chan = 0;
    // global PwmChanInfo pci;  // due to ISR not taking params

    stdio_init_all();
    sleep_ms(2000);  // takes time to init stdio 
    printf("\n\t\tApp: ac_sim_pico2w\n\n");

    // temp doing for debugging main(): TODO: REMOVEME 
    printf("Toggling scope pin\n");
    for (int x = 0; x < 20; ++x) {
        scope.toggle();
        sleep_ms(5);
    }

    printf("Setting wave params\n"); fflush(stdout);
    init_wave_params(g_pci);

    printf("Doing PWM config, then waiting 2 secs with dc = 1/2 Vdd:\n"); fflush(stdout);
    config_app_pwm(g_pci);
    sleep_ms(2000);

    printf("Entering main loop\n");
    printf("\n\n\n\n\n"); fflush(stdout); // make room for chanel info display 

    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    while (true) {
        scope.bin_out(true);
        //print_spinner();
        print_channel_info();
        //tight_loop_contents();
        //__wfi();
        scope.bin_out(false);
        sleep_ms(100);
    }
}
