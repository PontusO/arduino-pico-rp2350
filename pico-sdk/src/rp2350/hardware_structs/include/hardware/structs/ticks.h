// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_TICKS_H
#define _HARDWARE_STRUCTS_TICKS_H

#include "hardware/address_mapped.h"
#include "hardware/regs/ticks.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_ticks
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/ticks.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

enum tick_gen_index {
    TICK_PROC0,
    TICK_PROC1,
    TICK_TIMER0,
    TICK_TIMER1,
    TICK_WATCHDOG,
    TICK_COUNT
};

typedef struct {
    _REG_(TICKS_PROC0_CTRL_OFFSET) // TICKS_PROC0_CTRL
    // Controls the tick generator
    // 0x00000002 [1]     : RUNNING (0): Is the tick generator running?
    // 0x00000001 [0]     : ENABLE (0): start / stop tick generation
    io_rw_32 ctrl;

    _REG_(TICKS_PROC0_CYCLES_OFFSET) // TICKS_PROC0_CYCLES
    // 0x000001ff [8:0]   : PROC0_CYCLES (0): Total number of clk_tick cycles before the next tick
    io_rw_32 cycles;

    _REG_(TICKS_PROC0_COUNT_OFFSET) // TICKS_PROC0_COUNT
    // 0x000001ff [8:0]   : PROC0_COUNT (0): Count down timer: the remaining number clk_tick cycles before the next tick is...
    io_ro_32 count;
} tick_slice_hw_t;

typedef struct {
    tick_slice_hw_t ticks[5];

    _REG_(TICKS_RISCV_CTRL_OFFSET) // TICKS_RISCV_CTRL
    // Controls the tick generator
    // 0x00000002 [1]     : RUNNING (0): Is the tick generator running?
    // 0x00000001 [0]     : ENABLE (0): start / stop tick generation
    io_rw_32 riscv_ctrl;

    _REG_(TICKS_RISCV_CYCLES_OFFSET) // TICKS_RISCV_CYCLES
    // 0x000001ff [8:0]   : RISCV_CYCLES (0): Total number of clk_tick cycles before the next tick
    io_rw_32 riscv_cycles;

    _REG_(TICKS_RISCV_COUNT_OFFSET) // TICKS_RISCV_COUNT
    // 0x000001ff [8:0]   : RISCV_COUNT (0): Count down timer: the remaining number clk_tick cycles before the next tick is...
    io_ro_32 riscv_count;
} ticks_hw_t;

#define ticks_hw ((ticks_hw_t *)TICKS_BASE)

#endif
