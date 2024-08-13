// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_BOOTRAM_H
#define _HARDWARE_STRUCTS_BOOTRAM_H

#include "hardware/address_mapped.h"
#include "hardware/regs/bootram.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_bootram
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/bootram.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(BOOTRAM_WRITE_ONCE0_OFFSET) // BOOTRAM_WRITE_ONCE0
    // (Description copied from array index 0 register BOOTRAM_WRITE_ONCE0 applies similarly to other array indexes)
    //
    // This registers always ORs writes into its current contents
    io_rw_32 write_once[2];

    _REG_(BOOTRAM_BOOTLOCK_STAT_OFFSET) // BOOTRAM_BOOTLOCK_STAT
    // Bootlock status register
    // 0x000000ff [7:0]   : BOOTLOCK_STAT (0xff)
    io_rw_32 bootlock_stat;

    _REG_(BOOTRAM_BOOTLOCK0_OFFSET) // BOOTRAM_BOOTLOCK0
    // (Description copied from array index 0 register BOOTRAM_BOOTLOCK0 applies similarly to other array indexes)
    //
    // Read to claim and check
    io_rw_32 bootlock[8];
} bootram_hw_t;

#define bootram_hw ((bootram_hw_t *)(BOOTRAM_BASE + 0x800))

#endif
