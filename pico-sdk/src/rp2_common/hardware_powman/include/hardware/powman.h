#ifndef POWMAN_H_
#define POWMAN_H_

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "pico.h"

#include "hardware/structs/powman.h"


uint64_t powman_get_time_msec();
void powman_mask_debug_power_request(void);
void powman_timer_set(uint64_t time_msec);
void powman_timer_start(void);
void powman_timer_stop(void);
bool powman_timer_running(void);
void powman_timer_set_and_start(uint64_t time_msec);
void powman_timer_use_lposc();
void powman_timer_use_xosc();
void powman_timer_use_gpio1khz(uint32_t gpio);
void powman_timer_use_gpio1hz(uint32_t gpio);
void powman_print_time(uint64_t time_msec);

typedef struct {
    bool switched_core; // bit 3
    bool xip_cache; // bit 2
    bool sram_bank0; // bit 1
    bool sram_bank1; // bit 0
} powman_power_state;

void powman_get_power_state(powman_power_state *state);
bool powman_set_power_state(powman_power_state *state);
void powman_print_power_state(powman_power_state *state);
void powman_set_wakeup_alarm(uint64_t alarm_time_msec);
void powman_set_alarm(uint64_t alarm_time);
void powman_set_gpio_wakeup(uint32_t hw, uint32_t gpio, bool edge, bool high);
void powman_clear_pwrup_reqs(void);
uint32_t powman_get_pwrup_reqs(void);
bool powman_state_equal(powman_power_state *a, powman_power_state *b);
void powman_set_wakeup_state(powman_power_state *sleep_state, powman_power_state *wakeup_state);
void powman_clear_wakeups(void);

#define _POWMAN_PASSWORD 0x5AFE

static inline void powman_write(volatile uint32_t *reg, uint32_t value) {
    // Write needs a password in top 16 bits
    assert((value & 0xffff0000) == 0);
    value |= (_POWMAN_PASSWORD << 16);
    *reg = value;
}

static inline uint32_t powman_read(volatile uint32_t* reg) {
    return *reg;
}

static inline void powman_set_bits(volatile uint32_t *reg, uint32_t mask) {
    assert((mask & 0xffff0000) == 0);
    hw_set_bits(reg, (_POWMAN_PASSWORD << 16) | mask);
}

static inline void powman_clr_bits(volatile uint32_t *reg, uint32_t mask) {
    assert((mask & 0xffff0000) == 0);
    hw_clear_bits(reg, (_POWMAN_PASSWORD << 16) | mask);
}

void powman_enable_debug_printf(void);

#endif