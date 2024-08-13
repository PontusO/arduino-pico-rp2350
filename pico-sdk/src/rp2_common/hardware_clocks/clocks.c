/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico.h"
#include "hardware/regs/clocks.h"
#include "hardware/platform_defs.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "hardware/pll.h"
#include "hardware/xosc.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#if !PICO_RP2040
#include "hardware/structs/ticks.h"
#endif

// The RTC clock frequency is 48MHz divided by power of 2 (to ensure an integer
// division ratio will be used in the clocks block).  A divisor of 1024 generates
// an RTC clock tick of 46875Hz.  This frequency is relatively close to the
// customary 32 or 32.768kHz 'slow clock' crystals and provides good timing resolution.
#define RTC_CLOCK_FREQ_HZ       (USB_CLK_KHZ * KHZ / 1024)

check_hw_layout(clocks_hw_t, clk[clk_adc].selected, CLOCKS_CLK_ADC_SELECTED_OFFSET);
check_hw_layout(clocks_hw_t, fc0.result, CLOCKS_FC0_RESULT_OFFSET);
check_hw_layout(clocks_hw_t, ints, CLOCKS_INTS_OFFSET);

static uint32_t configured_freq[CLK_COUNT];

static resus_callback_t _resus_callback;

// Clock muxing consists of two components:
// - A glitchless mux, which can be switched freely, but whose inputs must be
//   free-running
// - An auxiliary (glitchy) mux, whose output glitches when switched, but has
//   no constraints on its inputs
// Not all clocks have both types of mux.
static inline bool has_glitchless_mux(enum clock_index clk_index) {
    return clk_index == clk_sys || clk_index == clk_ref;
}

void clock_stop(enum clock_index clk_index) {
    clock_hw_t *clock = &clocks_hw->clk[clk_index];
    hw_clear_bits(&clock->ctrl, CLOCKS_CLK_USB_CTRL_ENABLE_BITS);
    configured_freq[clk_index] = 0;
}

/// \tag::clock_configure[]
bool clock_configure(enum clock_index clk_index, uint32_t src, uint32_t auxsrc, uint32_t src_freq, uint32_t freq) {
    uint32_t div;

    assert(src_freq >= freq);

    if (freq > src_freq)
        return false;

    div = (uint32_t)((((uint64_t) src_freq) << CLOCKS_CLK_GPOUT0_DIV_INT_LSB) / freq);

    clock_hw_t *clock = &clocks_hw->clk[clk_index];

    // If increasing divisor, set divisor before source. Otherwise set source
    // before divisor. This avoids a momentary overspeed when e.g. switching
    // to a faster source and increasing divisor to compensate.
    if (div > clock->div)
        clock->div = div;

    // If switching a glitchless slice (ref or sys) to an aux source, switch
    // away from aux *first* to avoid passing glitches when changing aux mux.
    // Assume (!!!) glitchless source 0 is no faster than the aux source.
    if (has_glitchless_mux(clk_index) && src == CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX) {
        hw_clear_bits(&clock->ctrl, CLOCKS_CLK_REF_CTRL_SRC_BITS);
        while (!(clock->selected & 1u))
            tight_loop_contents();
    }
    // If no glitchless mux, cleanly stop the clock to avoid glitches
    // propagating when changing aux mux. Note it would be a really bad idea
    // to do this on one of the glitchless clocks (clk_sys, clk_ref).
    else {
        // Disable clock. On clk_ref and clk_sys this does nothing,
        // all other clocks have the ENABLE bit in the same position.
        hw_clear_bits(&clock->ctrl, CLOCKS_CLK_GPOUT0_CTRL_ENABLE_BITS);
        if (configured_freq[clk_index] > 0) {
            // Delay for 3 cycles of the target clock, for ENABLE propagation.
            // Note XOSC_COUNT is not helpful here because XOSC is not
            // necessarily running, nor is timer...
            uint delay_cyc = configured_freq[clk_sys] / configured_freq[clk_index] + 1;
            busy_wait_at_least_cycles(delay_cyc * 3);
        }
    }

    // Set aux mux first, and then glitchless mux if this clock has one
    hw_write_masked(&clock->ctrl,
        (auxsrc << CLOCKS_CLK_SYS_CTRL_AUXSRC_LSB),
        CLOCKS_CLK_SYS_CTRL_AUXSRC_BITS
    );

    if (has_glitchless_mux(clk_index)) {
        hw_write_masked(&clock->ctrl,
            src << CLOCKS_CLK_REF_CTRL_SRC_LSB,
            CLOCKS_CLK_REF_CTRL_SRC_BITS
        );
        while (!(clock->selected & (1u << src)))
            tight_loop_contents();
    }

    // Enable clock. On clk_ref and clk_sys this does nothing,
    // all other clocks have the ENABLE bit in the same position.
    hw_set_bits(&clock->ctrl, CLOCKS_CLK_GPOUT0_CTRL_ENABLE_BITS);

    // Now that the source is configured, we can trust that the user-supplied
    // divisor is a safe value.
    clock->div = div;

    // Store the configured frequency
    configured_freq[clk_index] = (uint32_t)((((uint64_t) src_freq) << CLOCKS_CLK_GPOUT0_DIV_INT_LSB) / div);

    return true;
}
/// \end::clock_configure[]

static void start_all_ticks(void) {
    uint32_t cycles = clock_get_hz(clk_ref) / MHZ;
#ifdef PICO_RP2040
    // RP2040 has a single tick generator in the watchdog which serves
    // watchdog, system timer and M0+ SysTick
    // The tick generator is clocked from clk_ref
    watchdog_start_tick(cycles);
#else
    // Multiple individual ticks. Set & start them all.
    for (int i = 0; i < (int)TICK_COUNT; ++i) {
        ticks_hw->ticks[i].cycles = cycles;
        ticks_hw->ticks[i].ctrl = TICKS_PROC0_CTRL_ENABLE_BITS;
    }
#endif
}

void clocks_init(void) {
    // Note: These need setting *before* the ticks are started
    if (running_on_fpga()) {
        for (uint i = 0; i < CLK_COUNT; i++) {
            configured_freq[i] = 48 * MHZ;
        }
        // clk_ref is 12MHz in both RP2040 and RP2350 FPGA
        configured_freq[clk_ref] = 12 * MHZ;
        // RP2040 has an extra clock, the rtc
#if PICO_RP2040
        configured_freq[clk_rtc] = RTC_CLOCK_FREQ_HZ;
#endif
    } else {
        // Disable resus that may be enabled from previous software
        clocks_hw->resus.ctrl = 0;

        // Enable the xosc
        xosc_init();

        // Before we touch PLLs, switch sys and ref cleanly away from their aux sources.
        hw_clear_bits(&clocks_hw->clk[clk_sys].ctrl, CLOCKS_CLK_SYS_CTRL_SRC_BITS);
        while (clocks_hw->clk[clk_sys].selected != 0x1)
            tight_loop_contents();
        hw_clear_bits(&clocks_hw->clk[clk_ref].ctrl, CLOCKS_CLK_REF_CTRL_SRC_BITS);
        while (clocks_hw->clk[clk_ref].selected != 0x1)
            tight_loop_contents();

        /// \tag::pll_init[]
        pll_init(pll_sys, PLL_COMMON_REFDIV, PLL_SYS_VCO_FREQ_KHZ * KHZ, PLL_SYS_POSTDIV1, PLL_SYS_POSTDIV2);
        pll_init(pll_usb, PLL_COMMON_REFDIV, PLL_USB_VCO_FREQ_KHZ * KHZ, PLL_USB_POSTDIV1, PLL_USB_POSTDIV2);
        /// \end::pll_init[]

        // Configure clocks

        // todo amy, what is this N1,2,4 meant to mean?
        // RP2040 CLK_REF = XOSC (usually) 12MHz / 1 = 12MHz
        // RP2350 CLK_REF = XOSC (XOSC_MHZ) / N (1,2,4) = 12MHz

        // clk_ref aux select is 0 because:
        //
        // - RP2040: no aux mux on clk_ref, so this field is don't-care.
        //
        // - RP2350: there is an aux mux, but we are selecting one of the
        //   non-aux inputs to the glitchless mux, so the aux select doesn't
        //   matter. The value of 0 here happens to be the sys PLL.

        clock_configure(clk_ref,
                        CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC,
                        0,
                        XOSC_KHZ * KHZ,
                        XOSC_KHZ * KHZ);

        /// \tag::configure_clk_sys[]
        // CLK SYS = PLL SYS (usually) 125MHz / 1 = 125MHz
        clock_configure(clk_sys,
                        CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                        CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                        SYS_CLK_KHZ * KHZ,
                        SYS_CLK_KHZ * KHZ);
        /// \end::configure_clk_sys[]

        // CLK USB = PLL USB 48MHz / 1 = 48MHz
        clock_configure(clk_usb,
                        0, // No GLMUX
                        CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                        USB_CLK_KHZ * KHZ,
                        USB_CLK_KHZ * KHZ);

        // CLK ADC = PLL USB 48MHZ / 1 = 48MHz
        clock_configure(clk_adc,
                        0, // No GLMUX
                        CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                        USB_CLK_KHZ * KHZ,
                        USB_CLK_KHZ * KHZ);

#if PICO_RP2040
        // CLK RTC = PLL USB 48MHz / 1024 = 46875Hz
        clock_configure(clk_rtc,
                        0, // No GLMUX
                        CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                        USB_CLK_KHZ * KHZ,
                        RTC_CLOCK_FREQ_HZ);
#endif

        // CLK PERI = clk_sys. Used as reference clock for UART and SPI serial.
        clock_configure(clk_peri,
                        0,
                        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                        SYS_CLK_KHZ * KHZ,
                        SYS_CLK_KHZ * KHZ);

#if PICO_RP2350
        // CLK_HSTX = clk_sys. Transmit bit clock for the HSTX peripheral.
        clock_configure(clk_hstx,
                        0,
                        CLOCKS_CLK_HSTX_CTRL_AUXSRC_VALUE_CLK_SYS,
                        SYS_CLK_KHZ * KHZ,
                        SYS_CLK_KHZ * KHZ);
#endif
    }

    // Finally, all clocks are configured so start the ticks
    // The ticks use clk_ref so now that is configured we can start them
    start_all_ticks();
}

/// \tag::clock_get_hz[]
uint32_t clock_get_hz(enum clock_index clk_index) {
    return configured_freq[clk_index];
}
/// \end::clock_get_hz[]

void clock_set_reported_hz(enum clock_index clk_index, uint hz) {
    configured_freq[clk_index] = hz;
}

/// \tag::frequency_count_khz[]
uint32_t frequency_count_khz(uint src) {
    fc_hw_t *fc = &clocks_hw->fc0;

    // If frequency counter is running need to wait for it. It runs even if the source is NULL
    while(fc->status & CLOCKS_FC0_STATUS_RUNNING_BITS) {
        tight_loop_contents();
    }

    // Set reference freq
    fc->ref_khz = clock_get_hz(clk_ref) / 1000;

    // FIXME: Don't pick random interval. Use best interval
    fc->interval = 10;

    // No min or max
    fc->min_khz = 0;
    fc->max_khz = 0xffffffff;

    // Set SRC which automatically starts the measurement
    fc->src = src;

    while(!(fc->status & CLOCKS_FC0_STATUS_DONE_BITS)) {
        tight_loop_contents();
    }

    // Return the result
    return fc->result >> CLOCKS_FC0_RESULT_KHZ_LSB;
}
/// \end::frequency_count_khz[]

static void clocks_handle_resus(void) {
    // Set clk_sys back to the ref clock rather than it being forced to clk_ref
    // by resus. Call the user's resus callback if they have set one

    // CLK SYS = CLK_REF. Must be running for this code to be running
    uint clk_ref_freq = clock_get_hz(clk_ref);
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF,
                    0,
                    clk_ref_freq,
                    clk_ref_freq);

    // Assert we have been resussed
    assert(clocks_hw->resus.status & CLOCKS_CLK_SYS_RESUS_STATUS_RESUSSED_BITS);

    // Now we have fixed clk_sys we can safely remove the resus
    hw_set_bits(&clocks_hw->resus.ctrl, CLOCKS_CLK_SYS_RESUS_CTRL_CLEAR_BITS);
    hw_clear_bits(&clocks_hw->resus.ctrl, CLOCKS_CLK_SYS_RESUS_CTRL_CLEAR_BITS);

    // Now we should no longer be resussed
    assert(!(clocks_hw->resus.status & CLOCKS_CLK_SYS_RESUS_STATUS_RESUSSED_BITS));

    // Call the user's callback to notify them of the resus event
    if (_resus_callback) {
        _resus_callback();
    }
}

static void clocks_irq_handler(void) {
    // Clocks interrupt handler. Only resus but handle irq
    // defensively just in case.
    uint32_t ints = clocks_hw->ints;

    if (ints & CLOCKS_INTE_CLK_SYS_RESUS_BITS) {
        ints &= ~CLOCKS_INTE_CLK_SYS_RESUS_BITS;
        clocks_handle_resus();
    }

#ifndef NDEBUG
    if (ints) {
        panic("Unexpected clocks irq\n");
    }
#endif
}

void clocks_enable_resus(resus_callback_t resus_callback) {
    // Restart clk_sys if it is stopped by forcing it
    // to the default source of clk_ref. If clk_ref stops running this will
    // not work.

    // Store user's resus callback
    _resus_callback = resus_callback;

    irq_set_exclusive_handler(CLOCKS_IRQ, clocks_irq_handler);

    // Enable the resus interrupt in clocks
    clocks_hw->inte = CLOCKS_INTE_CLK_SYS_RESUS_BITS;

    // Enable the clocks irq
    irq_set_enabled(CLOCKS_IRQ, true);

    // 2 * clk_ref freq / clk_sys_min_freq;
    // assume clk_ref is 3MHz and we want clk_sys to be no lower than 1MHz
    uint timeout = 2 * 3 * 1;

    // Enable resus with the maximum timeout
    clocks_hw->resus.ctrl = CLOCKS_CLK_SYS_RESUS_CTRL_ENABLE_BITS | timeout;
}

void clock_gpio_init_int_frac(uint gpio, uint src, uint32_t div_int, uint8_t div_frac) {
    // Bit messy but it's as much code to loop through a lookup
    // table. The sources for each gpout generators are the same
    // so just call with the sources from GP0
    uint gpclk = 0;
    if      (gpio == 21) gpclk = clk_gpout0;
    else if (gpio == 23) gpclk = clk_gpout1;
    else if (gpio == 24) gpclk = clk_gpout2;
    else if (gpio == 25) gpclk = clk_gpout3;
    else {
        invalid_params_if(CLOCKS, true);
    }

    // Set up the gpclk generator
    clocks_hw->clk[gpclk].ctrl = (src << CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_LSB) |
                                 CLOCKS_CLK_GPOUT0_CTRL_ENABLE_BITS;
    clocks_hw->clk[gpclk].div = (div_int << CLOCKS_CLK_GPOUT0_DIV_INT_LSB) | div_frac;

    // Set gpio pin to gpclock function
    gpio_set_function(gpio, GPIO_FUNC_GPCK);
}

static const uint8_t gpin0_src[CLK_COUNT] = {
    CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0, // CLK_GPOUT0
    CLOCKS_CLK_GPOUT1_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0, // CLK_GPOUT1
    CLOCKS_CLK_GPOUT2_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0, // CLK_GPOUT2
    CLOCKS_CLK_GPOUT3_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0, // CLK_GPOUT3
    CLOCKS_CLK_REF_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0,    // CLK_REF
    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0,    // CLK_SYS
    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0,   // CLK_PERI
    CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0,    // CLK_USB
    CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0,    // CLK_ADC
#if PICO_RP2040
    CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0,    // CLK_RTC
#endif
};

// Assert GPIN1 is GPIN0 + 1
static_assert(CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1 == (CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0 + 1), "hw mismatch");
static_assert(CLOCKS_CLK_GPOUT1_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1 == (CLOCKS_CLK_GPOUT1_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0 + 1), "hw mismatch");
static_assert(CLOCKS_CLK_GPOUT2_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1 == (CLOCKS_CLK_GPOUT2_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0 + 1), "hw mismatch");
static_assert(CLOCKS_CLK_GPOUT3_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1 == (CLOCKS_CLK_GPOUT3_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0 + 1), "hw mismatch");
static_assert(CLOCKS_CLK_REF_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1    == (CLOCKS_CLK_REF_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0    + 1), "hw mismatch");
static_assert(CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1    == (CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0    + 1), "hw mismatch");
static_assert(CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1   == (CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0   + 1), "hw mismatch");
static_assert(CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1    == (CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0    + 1), "hw mismatch");
static_assert(CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1    == (CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0    + 1), "hw mismatch");
#if PICO_RP2040
static_assert(CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_GPIN1    == (CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0    + 1), "hw mismatch");
#endif

bool clock_configure_gpin(enum clock_index clk_index, uint gpio, uint32_t src_freq, uint32_t freq) {
    // Configure a clock to run from a GPIO input
    uint gpin = 0;
    if      (gpio == 20) gpin = 0;
    else if (gpio == 22) gpin = 1;
    else {
        invalid_params_if(CLOCKS, true);
    }

    // Work out sources. GPIN is always an auxsrc
    uint src = 0;

    // GPIN1 == GPIN0 + 1
    uint auxsrc = gpin0_src[clk_index] + gpin;

    if (has_glitchless_mux(clk_index)) {
        // AUX src is always 1
        src = 1;
    }

    // Set the GPIO function
    gpio_set_function(gpio, GPIO_FUNC_GPCK);

    // Now we have the src, auxsrc, and configured the gpio input
    // call clock configure to run the clock from a gpio
    return clock_configure(clk_index, src, auxsrc, src_freq, freq);
}
