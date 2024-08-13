/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/claim.h"
#include "hardware/pio.h"
#include "hardware/pio_instructions.h"

// sanity check
check_hw_layout(pio_hw_t, sm[0].clkdiv, PIO_SM0_CLKDIV_OFFSET);
check_hw_layout(pio_hw_t, sm[1].clkdiv, PIO_SM1_CLKDIV_OFFSET);
check_hw_layout(pio_hw_t, instr_mem[0], PIO_INSTR_MEM0_OFFSET);
check_hw_layout(pio_hw_t, inte0, PIO_IRQ0_INTE_OFFSET);
check_hw_layout(pio_hw_t, txf[1], PIO_TXF1_OFFSET);
check_hw_layout(pio_hw_t, rxf[3], PIO_RXF3_OFFSET);
check_hw_layout(pio_hw_t, ints1, PIO_IRQ1_INTS_OFFSET);

static uint8_t claimed[(NUM_PIO_STATE_MACHINES * NUM_PIOS + 7) >> 3];

void pio_sm_claim(PIO pio, uint sm) {
    check_sm_param(sm);
    uint which = pio_get_index(pio);
    const char *msg =
#if PICO_PIO_VERSION > 0
        which == 2 ? "PIO 2 SM (%d - 8) already claimed" :
#endif
        which == 1 ? "PIO 1 SM (%d - 4) already claimed" :
                     "PIO 0 SM %d already claimed";
    hw_claim_or_assert(&claimed[0], which * NUM_PIO_STATE_MACHINES + sm, msg);
}

void pio_claim_sm_mask(PIO pio, uint sm_mask) {
    for(uint i = 0; sm_mask; i++, sm_mask >>= 1u) {
        if (sm_mask & 1u) pio_sm_claim(pio, i);
    }
}

void pio_sm_unclaim(PIO pio, uint sm) {
    check_sm_param(sm);
    uint which = pio_get_index(pio);
    hw_claim_clear(&claimed[0], which * NUM_PIO_STATE_MACHINES + sm);
}

int pio_claim_unused_sm(PIO pio, bool required) {
    // PIO index ranges from 0 to NUM_PIOS - 1.
    uint which = pio_get_index(pio);
    uint base = which * NUM_PIO_STATE_MACHINES;
    int index = hw_claim_unused_from_range((uint8_t*)&claimed[0], required, base,
                                      base + NUM_PIO_STATE_MACHINES - 1, "No PIO state machines are available");
    return index >= (int)base ? index - (int)base : -1;
}

bool pio_sm_is_claimed(PIO pio, uint sm) {
    check_sm_param(sm);
    uint which = pio_get_index(pio);
    return hw_is_claimed(&claimed[0], which * NUM_PIO_STATE_MACHINES + sm);
}

static_assert(PIO_INSTRUCTION_COUNT <= 32, "");
static uint32_t _used_instruction_space[NUM_PIOS];

static int find_offset_for_program(PIO pio, const pio_program_t *program) {
    assert(program->length <= PIO_INSTRUCTION_COUNT);
    uint32_t used_mask = _used_instruction_space[pio_get_index(pio)];
    uint32_t program_mask = (1u << program->length) - 1;
    if (program->origin >= 0) {
        if (program->origin > 32 - program->length) return -1;
        return used_mask & (program_mask << program->origin) ? -1 : program->origin;
    } else {
        // work down from the top always
        for (int i = 32 - program->length; i >= 0; i--) {
            if (!(used_mask & (program_mask << (uint) i))) {
                return i;
            }
        }
        return PICO_ERROR_INSUFFICIENT_RESOURCES;
    }
}

int pio_set_gpio_base(PIO pio, uint gpio_base) {
    invalid_params_if_and_return(PIO, gpio_base != 0 && (!PICO_PIO_VERSION || gpio_base != 16), PICO_ERROR_BAD_ALIGNMENT);
#if PICO_PIO_VERSION > 0
    uint32_t save = hw_claim_lock();
    uint32_t used_mask = _used_instruction_space[pio_get_index(pio)];
    invalid_params_if_and_return(PIO, used_mask, PICO_ERROR_INVALID_STATE);
    pio->gpiobase = gpio_base;
    hw_claim_unlock(save);
#else
    ((void)pio);
    ((void)gpio_base);
#endif
    return 0;
}

static bool is_gpio_compatible(PIO pio, const pio_program_t *program) {
#if PICO_PIO_VERSION > 0
    bool gpio_base = pio_get_gpio_base(pio);
    return !((gpio_base && (program->used_gpio_ranges & 1)) ||
             (!gpio_base && (program->used_gpio_ranges & 4)));
#else
    ((void)pio);
    ((void)program);
    return true;
#endif
}

static int add_program_at_offset_check(PIO pio, const pio_program_t *program, uint offset) {
    valid_params_if(PIO, offset < PIO_INSTRUCTION_COUNT);
    valid_params_if(PIO, offset + program->length <= PIO_INSTRUCTION_COUNT);
#if PICO_PIO_VERSION == 0
    if (program->pio_version) return PICO_ERROR_VERSION_MISMATCH;
#endif
    if (!is_gpio_compatible(pio, program)) return PICO_ERROR_BAD_ALIGNMENT; // todo better error?
    if (program->origin >= 0 && (uint)program->origin != offset) return PICO_ERROR_BAD_ALIGNMENT; // todo better error?
    uint32_t used_mask = _used_instruction_space[pio_get_index(pio)];
    uint32_t program_mask = (1u << program->length) - 1;
    return (used_mask & (program_mask << offset)) ? PICO_ERROR_INSUFFICIENT_RESOURCES : PICO_OK;
}

bool pio_can_add_program(PIO pio, const pio_program_t *program) {
    uint32_t save = hw_claim_lock();
    int rc = find_offset_for_program(pio, program);
    if (rc >= 0) rc = add_program_at_offset_check(pio, program, rc);
    hw_claim_unlock(save);
    return rc == 0;
}

bool pio_can_add_program_at_offset(PIO pio, const pio_program_t *program, uint offset) {
    uint32_t save = hw_claim_lock();
    bool rc = add_program_at_offset_check(pio, program, offset) == 0;
    hw_claim_unlock(save);
    return rc;
}

static int add_program_at_offset(PIO pio, const pio_program_t *program, int offset) {
    int rc = add_program_at_offset_check(pio, program, offset);
    if (rc != 0) return rc;
    for (uint i = 0; i < program->length; ++i) {
        uint16_t instr = program->instructions[i];
        pio->instr_mem[offset + i] = pio_instr_bits_jmp != _pio_major_instr_bits(instr) ? instr : instr + offset;
    }
    uint32_t program_mask = (1u << program->length) - 1;
    _used_instruction_space[pio_get_index(pio)] |= program_mask << offset;
    return offset;
}

// these assert if unable
int pio_add_program(PIO pio, const pio_program_t *program) {
    uint32_t save = hw_claim_lock();
    int offset = find_offset_for_program(pio, program);
    if (offset >= 0) {
        offset = add_program_at_offset(pio, program, offset);
    }
    hw_claim_unlock(save);
    return offset;
}

int pio_add_program_at_offset(PIO pio, const pio_program_t *program, uint offset) {
    uint32_t save = hw_claim_lock();
    int rc = add_program_at_offset(pio, program, (int)offset);
    hw_claim_unlock(save);
    return rc;
}

void pio_remove_program(PIO pio, const pio_program_t *program, uint loaded_offset) {
    uint32_t program_mask = (1u << program->length) - 1;
    program_mask <<= loaded_offset;
    uint32_t save = hw_claim_lock();
    assert(program_mask == (_used_instruction_space[pio_get_index(pio)] & program_mask));
    _used_instruction_space[pio_get_index(pio)] &= ~program_mask;
    hw_claim_unlock(save);
}

void pio_clear_instruction_memory(PIO pio) {
    uint32_t save = hw_claim_lock();
    _used_instruction_space[pio_get_index(pio)] = 0;
    for(uint i=0;i<PIO_INSTRUCTION_COUNT;i++) {
        pio->instr_mem[i] = pio_encode_jmp(i);
    }
    hw_claim_unlock(save);
}

// Set the value of all PIO pins. This is done by forcibly executing
// instructions on a "victim" state machine, sm. Ideally you should choose one
// which is not currently running a program. This is intended for one-time
// setup of initial pin states.
void pio_sm_set_pins(PIO pio, uint sm, uint32_t pins) {
    check_pio_param(pio);
    check_sm_param(sm);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    uint32_t execctrl_saved = pio->sm[sm].execctrl;
    hw_clear_bits(&pio->sm[sm].execctrl, 1u << PIO_SM0_EXECCTRL_OUT_STICKY_LSB);
    uint remaining = 32;
    uint base = 0;
    while (remaining) {
        uint decrement = remaining > 5 ? 5 : remaining;
        pio->sm[sm].pinctrl =
                (decrement << PIO_SM0_PINCTRL_SET_COUNT_LSB) |
                (base << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pins, pins & 0x1fu));
        remaining -= decrement;
        base += decrement;
        pins >>= 5;
    }
    pio->sm[sm].pinctrl = pinctrl_saved;
    pio->sm[sm].execctrl = execctrl_saved;
}

void pio_sm_set_pins_with_mask(PIO pio, uint sm, uint32_t pinvals, uint32_t pin_mask) {
    check_pio_param(pio);
    check_sm_param(sm);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    uint32_t execctrl_saved = pio->sm[sm].execctrl;
    hw_clear_bits(&pio->sm[sm].execctrl, 1u << PIO_SM0_EXECCTRL_OUT_STICKY_LSB);
    while (pin_mask) {
        uint base = (uint)__builtin_ctz(pin_mask);
        pio->sm[sm].pinctrl =
                (1u << PIO_SM0_PINCTRL_SET_COUNT_LSB) |
                (base << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pins, (pinvals >> base) & 0x1u));
        pin_mask &= pin_mask - 1;
    }
    pio->sm[sm].pinctrl = pinctrl_saved;
    pio->sm[sm].execctrl = execctrl_saved;
}

void pio_sm_set_pindirs_with_mask(PIO pio, uint sm, uint32_t pindirs, uint32_t pin_mask) {
    check_pio_param(pio);
    check_sm_param(sm);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    uint32_t execctrl_saved = pio->sm[sm].execctrl;
    hw_clear_bits(&pio->sm[sm].execctrl, 1u << PIO_SM0_EXECCTRL_OUT_STICKY_LSB);
    while (pin_mask) {
        uint base = (uint)__builtin_ctz(pin_mask);
        pio->sm[sm].pinctrl =
                (1u << PIO_SM0_PINCTRL_SET_COUNT_LSB) |
                (base << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pindirs, (pindirs >> base) & 0x1u));
        pin_mask &= pin_mask - 1;
    }
    pio->sm[sm].pinctrl = pinctrl_saved;
    pio->sm[sm].execctrl = execctrl_saved;
}

int pio_sm_set_consecutive_pindirs(PIO pio, uint sm, uint pin, uint count, bool is_out) {
    check_pio_param(pio);
    check_sm_param(sm);
    pin -= pio_get_gpio_base(pio);
    invalid_params_if_and_return(PIO, pin >= 32u, PICO_ERROR_INVALID_ARG);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    uint32_t execctrl_saved = pio->sm[sm].execctrl;
    hw_clear_bits(&pio->sm[sm].execctrl, 1u << PIO_SM0_EXECCTRL_OUT_STICKY_LSB);
    uint pindir_val = is_out ? 0x1f : 0;
    while (count > 5) {
        pio->sm[sm].pinctrl = (5u << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pindirs, pindir_val));
        count -= 5;
        pin = (pin + 5) & 0x1f;
    }
    pio->sm[sm].pinctrl = (count << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
    pio_sm_exec(pio, sm, pio_encode_set(pio_pindirs, pindir_val));
    pio->sm[sm].pinctrl = pinctrl_saved;
    pio->sm[sm].execctrl = execctrl_saved;
    return 0;
}

int pio_sm_init(PIO pio, uint sm, uint initial_pc, const pio_sm_config *config) {
    valid_params_if(PIO, initial_pc < PIO_INSTRUCTION_COUNT);
    // Halt the machine, set some sensible defaults
    pio_sm_set_enabled(pio, sm, false);

    int rc;
    if (config) {
        rc = pio_sm_set_config(pio, sm, config);
    } else {
        pio_sm_config c = pio_get_default_sm_config();
        rc = pio_sm_set_config(pio, sm, &c);
    }
    if (rc) return rc;

    pio_sm_clear_fifos(pio, sm);

    // Clear FIFO debug flags
    const uint32_t fdebug_sm_mask =
            (1u << PIO_FDEBUG_TXOVER_LSB) |
            (1u << PIO_FDEBUG_RXUNDER_LSB) |
            (1u << PIO_FDEBUG_TXSTALL_LSB) |
            (1u << PIO_FDEBUG_RXSTALL_LSB);
    pio->fdebug = fdebug_sm_mask << sm;

    // Finally, clear some internal SM state
    pio_sm_restart(pio, sm);
    pio_sm_clkdiv_restart(pio, sm);
    pio_sm_exec(pio, sm, pio_encode_jmp(initial_pc));
    return 0;
}

void pio_sm_drain_tx_fifo(PIO pio, uint sm) {
    uint instr = (pio->sm[sm].shiftctrl & PIO_SM0_SHIFTCTRL_AUTOPULL_BITS) ? pio_encode_out(pio_null, 32) :
                 pio_encode_pull(false, false);
    while (!pio_sm_is_tx_fifo_empty(pio, sm)) {
        pio_sm_exec(pio, sm, instr);
    }
}
