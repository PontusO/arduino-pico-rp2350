// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_PSM_H
#define _HARDWARE_STRUCTS_PSM_H

#include "hardware/address_mapped.h"
#include "hardware/regs/psm.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_psm
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/psm.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(PSM_FRCE_ON_OFFSET) // PSM_FRCE_ON
    // Force block out of reset (i
    // 0x01000000 [24]    : proc1 (0)
    // 0x00800000 [23]    : proc0 (0)
    // 0x00400000 [22]    : accessctrl (0)
    // 0x00200000 [21]    : sio (0)
    // 0x00100000 [20]    : xip (0)
    // 0x00080000 [19]    : sram9 (0)
    // 0x00040000 [18]    : sram8 (0)
    // 0x00020000 [17]    : sram7 (0)
    // 0x00010000 [16]    : sram6 (0)
    // 0x00008000 [15]    : sram5 (0)
    // 0x00004000 [14]    : sram4 (0)
    // 0x00002000 [13]    : sram3 (0)
    // 0x00001000 [12]    : sram2 (0)
    // 0x00000800 [11]    : sram1 (0)
    // 0x00000400 [10]    : sram0 (0)
    // 0x00000200 [9]     : bootram (0)
    // 0x00000100 [8]     : rom (0)
    // 0x00000080 [7]     : busfabric (0)
    // 0x00000040 [6]     : psm_ready (0)
    // 0x00000020 [5]     : clocks (0)
    // 0x00000010 [4]     : resets (0)
    // 0x00000008 [3]     : xosc (0)
    // 0x00000004 [2]     : rosc (0)
    // 0x00000002 [1]     : otp (0)
    // 0x00000001 [0]     : proc_cold (0)
    io_rw_32 frce_on;

    _REG_(PSM_FRCE_OFF_OFFSET) // PSM_FRCE_OFF
    // Force into reset (i
    // 0x01000000 [24]    : proc1 (0)
    // 0x00800000 [23]    : proc0 (0)
    // 0x00400000 [22]    : accessctrl (0)
    // 0x00200000 [21]    : sio (0)
    // 0x00100000 [20]    : xip (0)
    // 0x00080000 [19]    : sram9 (0)
    // 0x00040000 [18]    : sram8 (0)
    // 0x00020000 [17]    : sram7 (0)
    // 0x00010000 [16]    : sram6 (0)
    // 0x00008000 [15]    : sram5 (0)
    // 0x00004000 [14]    : sram4 (0)
    // 0x00002000 [13]    : sram3 (0)
    // 0x00001000 [12]    : sram2 (0)
    // 0x00000800 [11]    : sram1 (0)
    // 0x00000400 [10]    : sram0 (0)
    // 0x00000200 [9]     : bootram (0)
    // 0x00000100 [8]     : rom (0)
    // 0x00000080 [7]     : busfabric (0)
    // 0x00000040 [6]     : psm_ready (0)
    // 0x00000020 [5]     : clocks (0)
    // 0x00000010 [4]     : resets (0)
    // 0x00000008 [3]     : xosc (0)
    // 0x00000004 [2]     : rosc (0)
    // 0x00000002 [1]     : otp (0)
    // 0x00000001 [0]     : proc_cold (0)
    io_rw_32 frce_off;

    _REG_(PSM_WDSEL_OFFSET) // PSM_WDSEL
    // Set to 1 if the watchdog should reset this
    // 0x01000000 [24]    : proc1 (0)
    // 0x00800000 [23]    : proc0 (0)
    // 0x00400000 [22]    : accessctrl (0)
    // 0x00200000 [21]    : sio (0)
    // 0x00100000 [20]    : xip (0)
    // 0x00080000 [19]    : sram9 (0)
    // 0x00040000 [18]    : sram8 (0)
    // 0x00020000 [17]    : sram7 (0)
    // 0x00010000 [16]    : sram6 (0)
    // 0x00008000 [15]    : sram5 (0)
    // 0x00004000 [14]    : sram4 (0)
    // 0x00002000 [13]    : sram3 (0)
    // 0x00001000 [12]    : sram2 (0)
    // 0x00000800 [11]    : sram1 (0)
    // 0x00000400 [10]    : sram0 (0)
    // 0x00000200 [9]     : bootram (0)
    // 0x00000100 [8]     : rom (0)
    // 0x00000080 [7]     : busfabric (0)
    // 0x00000040 [6]     : psm_ready (0)
    // 0x00000020 [5]     : clocks (0)
    // 0x00000010 [4]     : resets (0)
    // 0x00000008 [3]     : xosc (0)
    // 0x00000004 [2]     : rosc (0)
    // 0x00000002 [1]     : otp (0)
    // 0x00000001 [0]     : proc_cold (0)
    io_rw_32 wdsel;

    _REG_(PSM_DONE_OFFSET) // PSM_DONE
    // Is the subsystem ready?
    // 0x01000000 [24]    : proc1 (0)
    // 0x00800000 [23]    : proc0 (0)
    // 0x00400000 [22]    : accessctrl (0)
    // 0x00200000 [21]    : sio (0)
    // 0x00100000 [20]    : xip (0)
    // 0x00080000 [19]    : sram9 (0)
    // 0x00040000 [18]    : sram8 (0)
    // 0x00020000 [17]    : sram7 (0)
    // 0x00010000 [16]    : sram6 (0)
    // 0x00008000 [15]    : sram5 (0)
    // 0x00004000 [14]    : sram4 (0)
    // 0x00002000 [13]    : sram3 (0)
    // 0x00001000 [12]    : sram2 (0)
    // 0x00000800 [11]    : sram1 (0)
    // 0x00000400 [10]    : sram0 (0)
    // 0x00000200 [9]     : bootram (0)
    // 0x00000100 [8]     : rom (0)
    // 0x00000080 [7]     : busfabric (0)
    // 0x00000040 [6]     : psm_ready (0)
    // 0x00000020 [5]     : clocks (0)
    // 0x00000010 [4]     : resets (0)
    // 0x00000008 [3]     : xosc (0)
    // 0x00000004 [2]     : rosc (0)
    // 0x00000002 [1]     : otp (0)
    // 0x00000001 [0]     : proc_cold (0)
    io_ro_32 done;
} psm_hw_t;

#define psm_hw ((psm_hw_t *)PSM_BASE)

#endif
