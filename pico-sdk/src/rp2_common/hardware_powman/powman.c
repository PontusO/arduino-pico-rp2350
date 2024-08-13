#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "pico.h"

#include "hardware/gpio.h"
#include "hardware/powman.h"

#ifndef PICO_POWMAN_DEBUG
#define PICO_POWMAN_DEBUG 0
#endif

#if PICO_POWMAN_DEBUG
bool powman_debug_printf = false;
void powman_enable_debug_printf(void) {
    powman_debug_printf = true;
}
#define powman_debug(format, args...) if (powman_debug_printf) printf(format, ## args)
#else
#define powman_debug(...)
#endif

void powman_mask_debug_power_request() {
    powman_set_bits(&powman_hw->dbg_pwrcfg, 1);
}

void powman_print_time(uint64_t time_msec) {
    powman_debug("0x%0"PRIx32"_%0"PRIx32" \n", (uint32_t)(time_msec >> 32), (uint32_t)(time_msec & 0xffffffff));
}

bool powman_timer_running(void) {
    return powman_hw->timer & POWMAN_TIMER_RUN_BITS;
}

void powman_timer_stop(void) {
    powman_clr_bits(&powman_hw->timer, POWMAN_TIMER_RUN_BITS);
}

void powman_timer_start(void) {
    powman_set_bits(&powman_hw->timer, POWMAN_TIMER_RUN_BITS);
}

void powman_timer_set(uint64_t time_msec) {
    bool was_running = powman_timer_running();
    if (was_running) powman_timer_stop();
    powman_write(&powman_hw->set_time_15to0, time_msec & 0xffff);
    powman_write(&powman_hw->set_time_31to16, (time_msec >> 16) & 0xffff);
    powman_write(&powman_hw->set_time_47to32, (time_msec >> 32) & 0xffff);
    powman_write(&powman_hw->set_time_63to48, (time_msec >> 48) & 0xffff);
    if (was_running) powman_timer_start();
}

void powman_timer_set_and_start(uint64_t time_msec) {
    powman_timer_set(time_msec);
    powman_timer_start();
}

static uint64_t powman_get_time_raw() {
    return ((uint64_t)powman_hw->read_time_lower) | ((uint64_t)powman_hw->read_time_upper << 32);
}

uint64_t powman_get_time_msec() {
    // Read time and make sure the upper time registers haven't rolled over
    uint64_t time1, time2;
    do {
        time1 = powman_get_time_raw();
        time2 = powman_get_time_raw();
    } while (time1 != time2);
    return time2;
}

void powman_timer_use_xosc() {
    powman_write(&powman_hw->xosc_freq_khz_int, XOSC_MHZ * 1000);
    powman_set_bits(&powman_hw->timer, POWMAN_TIMER_USE_XOSC_BITS);
    // If running wait for it to switch
    if (powman_timer_running()) {
        while(!(powman_hw->timer & POWMAN_TIMER_USING_XOSC_BITS));
    }
}

void powman_timer_use_lposc() {
    powman_set_bits(&powman_hw->timer, POWMAN_TIMER_USE_LPOSC_BITS);
    // If running wait for it to switch
    if (powman_timer_running()) {
        while(!(powman_hw->timer & POWMAN_TIMER_USING_LPOSC_BITS));
    }
}

void powman_get_power_state(powman_power_state *state) {
    uint32_t state_reg = powman_hw->state & POWMAN_STATE_CURRENT_BITS;
    state->sram_bank1 = (state_reg & (1 << 0)) == 0;
    state->sram_bank0 = (state_reg & (1 << 1)) >> 1 == 0;
    state->xip_cache = (state_reg & (1 << 2))  >> 2 == 0;
    state->switched_core = (state_reg & (1 << 3)) >> 3 == 0;
}

void powman_print_power_state(powman_power_state *state) {
    powman_debug("Power state: switched core %d, xip_cache %d, sram_bank0 %d, sram_bank1 %d\n",
        state->switched_core,
        state->xip_cache,
        state->sram_bank0,
        state->sram_bank1
    );
}

// TODO: Should this fail to go to sleep if there is no wakeup alarm
bool powman_set_power_state(powman_power_state *state) {
    // Clear req ignored in case it has been set
    powman_clr_bits(&powman_hw->state, POWMAN_STATE_REQ_IGNORED_BITS);

    uint8_t state_bits = ((!state->switched_core) << 3) |
                         ((!state->xip_cache)     << 2) |
                         ((!state->sram_bank0)    << 1) |
                         (!state->sram_bank1);
    powman_debug("powman: Requesting state %x\n", state_bits);
    powman_write(&powman_hw->state, state_bits << POWMAN_STATE_REQ_LSB);

    // Has it been ignored?
    if (powman_hw->state & POWMAN_STATE_REQ_IGNORED_BITS) {
        powman_debug("State req ignored because of a pending pwrup req: %"PRIx32"\n", powman_hw->current_pwrup_req);
        return false;
    }

    bool state_valid = (powman_hw->state & POWMAN_STATE_BAD_SW_REQ_BITS) == 0;
    if (!state_valid) {
        powman_debug("powman: Requested state invalid\n");
        return false;
    } else {
        powman_debug("powman: Requested state valid\n");
    }
    if (state->switched_core == 0) {
        // If we are turning off switched core then POWMAN_STATE_WAITING_BITS will be
        // set because we are waiting for proc to go to sleep, so return true and then the proc
        // can go to sleep

        // Note if the powerdown is being blocked by a pending pwrup request we will break out of this with a return false

        // Clk pow is slow so can take a few clk_pow cycles for waiting to turn up
        for (int i = 0; i < 100; i++) {
            if (powman_hw->state & POWMAN_STATE_WAITING_BITS) {
                return true;
            }
        }

        // If it hasn't turned up then false
        powman_debug("powman: STATE_WAITING hasn't turned up\n");
        return false;
    }
    // Wait while the state is changing then return true as we will be in the new state
    powman_debug("powman: waiting for state change\n");
    while(powman_hw->state & POWMAN_STATE_CHANGING_BITS);
    powman_debug("powman: state changed to %x\n", state_bits);
    return true;
}

void powman_set_wakeup_state(powman_power_state *sleep_state, powman_power_state *wakeup_state) {
    // When powman wakes up it can keep the state of the sram0 and sram1 banks. Note, it can't
    // explicitly
    assert(wakeup_state->xip_cache);
    assert(wakeup_state->switched_core);
    assert(sleep_state->sram_bank0 == wakeup_state->sram_bank0);
    assert(sleep_state->sram_bank1 == wakeup_state->sram_bank1);
    powman_clr_bits(&powman_hw->seq_cfg, POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_BITS | POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_BITS);
    uint32_t seq_cfg_set = 0;
    if (!sleep_state->sram_bank0) seq_cfg_set |= POWMAN_SEQ_CFG_HW_PWRUP_SRAM0_BITS;
    if (!sleep_state->sram_bank1) seq_cfg_set |= POWMAN_SEQ_CFG_HW_PWRUP_SRAM1_BITS;
    powman_set_bits(&powman_hw->seq_cfg, seq_cfg_set);
}

void powman_set_alarm(uint64_t alarm_time) {
    powman_set_bits(&powman_hw->inte, POWMAN_INTE_TIMER_BITS);
    powman_clr_bits(&powman_hw->timer, POWMAN_TIMER_ALARM_ENAB_BITS);
    // Alarm must be disabled to set the alarm time
    powman_write(&powman_hw->alarm_time_15to0, alarm_time & 0xffff);
    powman_write(&powman_hw->alarm_time_31to16, (alarm_time >> 16) & 0xffff);
    powman_write(&powman_hw->alarm_time_47to32, (alarm_time >> 32) & 0xffff);
    powman_write(&powman_hw->alarm_time_63to48, (alarm_time >> 48) & 0xffff);
    // TODO: Assuming pwrup on alarm has no bad side effects if already powered up
    powman_set_bits(&powman_hw->timer, POWMAN_TIMER_ALARM_ENAB_BITS);
}

void powman_set_wakeup_alarm(uint64_t alarm_time) {
    powman_set_alarm(alarm_time);
    powman_set_bits(&powman_hw->timer, POWMAN_TIMER_PWRUP_ON_ALARM_BITS);
}

void powman_set_gpio_wakeup(uint32_t hw, uint32_t gpio, bool edge, bool high) {
    // Need to make sure pad is input enabled
    gpio_set_input_enabled(gpio, true);

    // Set up gpio hardware for what we want
    uint32_t pwrup = (edge ? POWMAN_PWRUP0_MODE_VALUE_EDGE : POWMAN_PWRUP0_MODE_VALUE_LEVEL) << POWMAN_PWRUP0_MODE_LSB;
    pwrup |= (high ? POWMAN_PWRUP0_DIRECTION_BITS : 0);
    pwrup |= gpio << POWMAN_PWRUP0_SOURCE_LSB;
    powman_write(&powman_hw->pwrup[hw], pwrup);

    // Clear the status bit in case an edge is already latched
    powman_clr_bits(&powman_hw->pwrup[hw], POWMAN_PWRUP0_STATUS_BITS);

    // Important to enable it separately to allow the gpio to change
    powman_set_bits(&powman_hw->pwrup[hw], POWMAN_PWRUP0_ENABLE_BITS);
}

void powman_clear_wakeups(void) {
    for (int i = 0; i < 4; i++) {
        powman_clr_bits(&powman_hw->pwrup[i], POWMAN_PWRUP0_ENABLE_BITS);
    }
    powman_clr_bits(&powman_hw->timer, POWMAN_TIMER_ALARM_ENAB_BITS | POWMAN_TIMER_PWRUP_ON_ALARM_BITS);
}

bool powman_state_equal(powman_power_state *a, powman_power_state *b) {
    return (a->switched_core == b->switched_core) &&
           (a->xip_cache     == b->xip_cache    ) &&
           (a->sram_bank0    == b->sram_bank0   ) &&
           (a->sram_bank1    == b->sram_bank1   ) ;
}

static void powman_timer_use_gpio(uint32_t gpio, uint32_t use, uint32_t using) {
    bool was_running = powman_timer_running();
    if (was_running) powman_timer_stop();
    // Which gpio
    if (gpio > 47) assert(0);
    gpio_set_input_enabled(gpio, true);
    powman_write(&powman_hw->ext_time_ref, gpio);
    powman_set_bits(&powman_hw->timer, use);
    if (was_running) {
        powman_timer_start();
    while(!(powman_hw->timer & using));
    }
}

void powman_timer_use_gpio1khz(uint32_t gpio) {
    powman_timer_use_gpio(gpio, POWMAN_TIMER_USE_GPIO_1KHZ_BITS, POWMAN_TIMER_USING_GPIO_1KHZ_BITS);
}

void powman_timer_use_gpio1hz(uint32_t gpio) {
    powman_timer_use_gpio(gpio, POWMAN_TIMER_USE_GPIO_1HZ_BITS, POWMAN_TIMER_USING_GPIO_1HZ_BITS);
}