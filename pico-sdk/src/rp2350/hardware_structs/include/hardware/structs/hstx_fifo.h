// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_HSTX_FIFO_H
#define _HARDWARE_STRUCTS_HSTX_FIFO_H

#include "hardware/address_mapped.h"
#include "hardware/regs/hstx_fifo.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_hstx_fifo
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/hstx_fifo.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(HSTX_FIFO_STAT_OFFSET) // HSTX_FIFO_STAT
    // FIFO status
    // 0x00000400 [10]    : WOF (0): FIFO was written when full
    // 0x00000200 [9]     : EMPTY (0)
    // 0x00000100 [8]     : FULL (0)
    // 0x000000ff [7:0]   : LEVEL (0)
    io_rw_32 stat;

    _REG_(HSTX_FIFO_FIFO_OFFSET) // HSTX_FIFO_FIFO
    // Write access to FIFO
    io_wo_32 fifo;
} hstx_fifo_hw_t;

#define hstx_fifo_hw ((hstx_fifo_hw_t *)HSTX_FIFO_BASE)

#endif
