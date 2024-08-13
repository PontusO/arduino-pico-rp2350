// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_XIP_CTRL_H
#define _HARDWARE_STRUCTS_XIP_CTRL_H

#include "hardware/address_mapped.h"
#include "hardware/regs/xip.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_xip
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/xip.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(XIP_CTRL_OFFSET) // XIP_CTRL
    // Cache control register
    // 0x00000800 [11]    : WRITABLE_M1 (0): If 1, enable writes to XIP memory window 1 (addresses 0x11000000 through...
    // 0x00000400 [10]    : WRITABLE_M0 (0): If 1, enable writes to XIP memory window 0 (addresses 0x10000000 through...
    // 0x00000200 [9]     : SPLIT_WAYS (0): When 1, route all cached+Secure accesses to way 0 of the cache, and route all...
    // 0x00000100 [8]     : MAINT_NONSEC (0): When 0, NonSecure accesses to the cache maintenance address window (addr[27]...
    // 0x00000080 [7]     : NO_UNTRANSLATED_NONSEC (1): When 1, NonSecure accesses to the uncached, untranslated window...
    // 0x00000040 [6]     : NO_UNTRANSLATED_SEC (0): When 1, Secure accesses to the uncached, untranslated window...
    // 0x00000020 [5]     : NO_UNCACHED_NONSEC (0): When 1, NonSecure accesses to the uncached window (addr[27:26] == 1)...
    // 0x00000010 [4]     : NO_UNCACHED_SEC (0): When 1, Secure accesses to the uncached window (addr[27:26] == 1) will...
    // 0x00000008 [3]     : POWER_DOWN (0): When 1, the cache memories are powered down
    // 0x00000002 [1]     : EN_NONSECURE (1): When 1, enable the cache for NonSecure accesses
    // 0x00000001 [0]     : EN_SECURE (1): When 1, enable the cache for Secure accesses
    io_rw_32 ctrl;

    uint32_t _pad0;

    _REG_(XIP_STAT_OFFSET) // XIP_STAT
    // 0x00000004 [2]     : FIFO_FULL (0): When 1, indicates the XIP streaming FIFO is completely full
    // 0x00000002 [1]     : FIFO_EMPTY (1): When 1, indicates the XIP streaming FIFO is completely empty
    io_ro_32 stat;

    _REG_(XIP_CTR_HIT_OFFSET) // XIP_CTR_HIT
    // Cache Hit counter
    io_rw_32 ctr_hit;

    _REG_(XIP_CTR_ACC_OFFSET) // XIP_CTR_ACC
    // Cache Access counter
    io_rw_32 ctr_acc;

    _REG_(XIP_STREAM_ADDR_OFFSET) // XIP_STREAM_ADDR
    // FIFO stream address
    // 0xfffffffc [31:2]  : STREAM_ADDR (0): The address of the next word to be streamed from flash to the streaming FIFO
    io_rw_32 stream_addr;

    _REG_(XIP_STREAM_CTR_OFFSET) // XIP_STREAM_CTR
    // FIFO stream control
    // 0x003fffff [21:0]  : STREAM_CTR (0): Write a nonzero value to start a streaming read
    io_rw_32 stream_ctr;

    _REG_(XIP_STREAM_FIFO_OFFSET) // XIP_STREAM_FIFO
    // FIFO stream data
    io_ro_32 stream_fifo;
} xip_ctrl_hw_t;

#define xip_ctrl_hw ((xip_ctrl_hw_t *)XIP_CTRL_BASE)

#define XIP_STAT_FIFO_FULL XIP_STAT_FIFO_FULL_BITS
#define XIP_STAT_FIFO_EMPTY XIP_STAT_FIFO_EMPTY_BITS
#define XIP_STAT_FLUSH_RDY XIP_STAT_FLUSH_READY_BITS

#endif
