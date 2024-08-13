/*
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/rand.h"
#include "hardware/timer.h"
#include "hardware/structs/trng.h"

// todo amy this needs to be threadsafe - also it might move to bootrom?

// These coefficients have been chosen following on-chip characterisation
#define TRNG_ROSC_CHAIN_LEN_SEL     1
#define CLOCKS_PER_ROSC_SAMPLE      20

// microsecond loop escape times to avoid wedging
#define ESCAPE_TIME_US_POST_RESET   10
#define ESCAPE_TIME_US_TRNG_VALID   5000

static void trng_sw_reset(uint32_t sample_cnt1) {
    bool pass = false;

    // clear any lingering interrupt status - the sw reset doesn't do this!
    trng_hw->rng_icr = ~0ul;

    uint32_t esc_time = time_us_32() + ESCAPE_TIME_US_POST_RESET;

    trng_hw->trng_sw_reset = 1;  // takes 4 cycles to complete & self clears

    do {
        trng_hw->sample_cnt1 = sample_cnt1;
        if (sample_cnt1 == trng_hw->sample_cnt1) {
            pass = true;
            break;
        }
    } while (0 < (int) (esc_time - time_us_32()));

    hard_assert(pass == true);
}


static void trng_generate_entropy(void) {
    bool pass = false;

    trng_sw_reset(CLOCKS_PER_ROSC_SAMPLE);

    trng_hw->trng_config = TRNG_ROSC_CHAIN_LEN_SEL;

    // FIXME HERE TODO remove in time
    // trng_debug_control default resets to 0, i.e. activate all result checks.
    // For FPGA testing, disable the autocorrelator checking
    //if (running_on_fpga()) {
    if (running_in_sim() || running_on_fpga()) {
        trng_hw->rng_debug_en_input = 1;
        trng_hw->trng_debug_control = TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_BITS;
    }

    trng_hw->rnd_source_enable = 1;

    uint32_t esc_time = time_us_32() + ESCAPE_TIME_US_TRNG_VALID;

    // check for a valid result, detected errors or a timeout ...
    while (1) {
        uint32_t rng_isr = trng_hw->rng_isr;

        if (rng_isr == TRNG_RNG_ISR_EHR_VALID_BITS) {
            pass = true;
            break;
        }

        // Check for errors
        if (rng_isr & (~TRNG_RNG_ISR_EHR_VALID_BITS)) {
            break;
        }

        if (0 < (int) (time_us_32() - esc_time)) {
            break;
        }
    }

    hard_assert(pass == true);
    trng_hw->rnd_source_enable = 0;
}

// NOTE:  All the functions below must read the *last* 'ehr_data' register,
// that is ehr_data[5], as the last thing they do since the read clears
// all the ehr registers and the valid status bit.

void get_rand_128(rng_128_t *ptr128) {
    trng_generate_entropy();
    ptr128->r[0] = (uint64_t) trng_hw->ehr_data[2] << 32 | trng_hw->ehr_data[3];
    ptr128->r[1] = (uint64_t) trng_hw->ehr_data[4] << 32 | trng_hw->ehr_data[5];
}


uint64_t get_rand_64(void) {
    trng_generate_entropy();
    return (uint64_t) trng_hw->ehr_data[4] << 32 | trng_hw->ehr_data[5];
}


uint32_t get_rand_32(void) {
    trng_generate_entropy();
    return trng_hw->ehr_data[5];
}
