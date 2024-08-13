// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_IOBANK0_H
#define _HARDWARE_STRUCTS_IOBANK0_H

#include "hardware/address_mapped.h"
#include "hardware/regs/io_bank0.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_io_bank0
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/io_bank0.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(IO_BANK0_GPIO0_STATUS_OFFSET) // IO_BANK0_GPIO0_STATUS
    // 0x04000000 [26]    : IRQTOPROC (0): interrupt to processors, after override is applied
    // 0x00020000 [17]    : INFROMPAD (0): input signal from pad, before filtering and override are applied
    // 0x00002000 [13]    : OETOPAD (0): output enable to pad after register overide is applied
    // 0x00000200 [9]     : OUTTOPAD (0): output signal to pad after register overide is applied
    io_ro_32 status;

    _REG_(IO_BANK0_GPIO0_CTRL_OFFSET) // IO_BANK0_GPIO0_CTRL
    // 0x30000000 [29:28] : IRQOVER (0)
    // 0x00030000 [17:16] : INOVER (0)
    // 0x0000c000 [15:14] : OEOVER (0)
    // 0x00003000 [13:12] : OUTOVER (0)
    // 0x0000001f [4:0]   : FUNCSEL (0x1f): 0-31 -> selects pin function according to the gpio table
    io_rw_32 ctrl;
} iobank0_status_ctrl_hw_t;

typedef struct {
    _REG_(IO_BANK0_PROC0_INTE0_OFFSET) // IO_BANK0_PROC0_INTE0
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTE0 applies similarly to other array indexes)
    //
    // Interrupt Enable for proc0
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_rw_32 inte[6];

    _REG_(IO_BANK0_PROC0_INTF0_OFFSET) // IO_BANK0_PROC0_INTF0
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTF0 applies similarly to other array indexes)
    //
    // Interrupt Force for proc0
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_rw_32 intf[6];

    _REG_(IO_BANK0_PROC0_INTS0_OFFSET) // IO_BANK0_PROC0_INTS0
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTS0 applies similarly to other array indexes)
    //
    // Interrupt status after masking & forcing for proc0
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_ro_32 ints[6];
} io_irq_ctrl_hw_t;

/// \tag::iobank0_hw[]
typedef struct {
    iobank0_status_ctrl_hw_t io[NUM_BANK0_GPIOS]; // 48

    uint32_t _pad0[32];

    _REG_(IO_BANK0_IRQSUMMARY_PROC0_SECURE0_OFFSET) // IO_BANK0_IRQSUMMARY_PROC0_SECURE0
    // (Description copied from array index 0 register IO_BANK0_IRQSUMMARY_PROC0_SECURE0 applies similarly to other array indexes)
    //
    // 0x80000000 [31]    : GPIO31 (0)
    // 0x40000000 [30]    : GPIO30 (0)
    // 0x20000000 [29]    : GPIO29 (0)
    // 0x10000000 [28]    : GPIO28 (0)
    // 0x08000000 [27]    : GPIO27 (0)
    // 0x04000000 [26]    : GPIO26 (0)
    // 0x02000000 [25]    : GPIO25 (0)
    // 0x01000000 [24]    : GPIO24 (0)
    // 0x00800000 [23]    : GPIO23 (0)
    // 0x00400000 [22]    : GPIO22 (0)
    // 0x00200000 [21]    : GPIO21 (0)
    // 0x00100000 [20]    : GPIO20 (0)
    // 0x00080000 [19]    : GPIO19 (0)
    // 0x00040000 [18]    : GPIO18 (0)
    // 0x00020000 [17]    : GPIO17 (0)
    // 0x00010000 [16]    : GPIO16 (0)
    // 0x00008000 [15]    : GPIO15 (0)
    // 0x00004000 [14]    : GPIO14 (0)
    // 0x00002000 [13]    : GPIO13 (0)
    // 0x00001000 [12]    : GPIO12 (0)
    // 0x00000800 [11]    : GPIO11 (0)
    // 0x00000400 [10]    : GPIO10 (0)
    // 0x00000200 [9]     : GPIO9 (0)
    // 0x00000100 [8]     : GPIO8 (0)
    // 0x00000080 [7]     : GPIO7 (0)
    // 0x00000040 [6]     : GPIO6 (0)
    // 0x00000020 [5]     : GPIO5 (0)
    // 0x00000010 [4]     : GPIO4 (0)
    // 0x00000008 [3]     : GPIO3 (0)
    // 0x00000004 [2]     : GPIO2 (0)
    // 0x00000002 [1]     : GPIO1 (0)
    // 0x00000001 [0]     : GPIO0 (0)
    io_ro_32 irqsummary_proc0_secure[2];

    _REG_(IO_BANK0_IRQSUMMARY_PROC0_NONSECURE0_OFFSET) // IO_BANK0_IRQSUMMARY_PROC0_NONSECURE0
    // (Description copied from array index 0 register IO_BANK0_IRQSUMMARY_PROC0_NONSECURE0 applies similarly to other array indexes)
    //
    // 0x80000000 [31]    : GPIO31 (0)
    // 0x40000000 [30]    : GPIO30 (0)
    // 0x20000000 [29]    : GPIO29 (0)
    // 0x10000000 [28]    : GPIO28 (0)
    // 0x08000000 [27]    : GPIO27 (0)
    // 0x04000000 [26]    : GPIO26 (0)
    // 0x02000000 [25]    : GPIO25 (0)
    // 0x01000000 [24]    : GPIO24 (0)
    // 0x00800000 [23]    : GPIO23 (0)
    // 0x00400000 [22]    : GPIO22 (0)
    // 0x00200000 [21]    : GPIO21 (0)
    // 0x00100000 [20]    : GPIO20 (0)
    // 0x00080000 [19]    : GPIO19 (0)
    // 0x00040000 [18]    : GPIO18 (0)
    // 0x00020000 [17]    : GPIO17 (0)
    // 0x00010000 [16]    : GPIO16 (0)
    // 0x00008000 [15]    : GPIO15 (0)
    // 0x00004000 [14]    : GPIO14 (0)
    // 0x00002000 [13]    : GPIO13 (0)
    // 0x00001000 [12]    : GPIO12 (0)
    // 0x00000800 [11]    : GPIO11 (0)
    // 0x00000400 [10]    : GPIO10 (0)
    // 0x00000200 [9]     : GPIO9 (0)
    // 0x00000100 [8]     : GPIO8 (0)
    // 0x00000080 [7]     : GPIO7 (0)
    // 0x00000040 [6]     : GPIO6 (0)
    // 0x00000020 [5]     : GPIO5 (0)
    // 0x00000010 [4]     : GPIO4 (0)
    // 0x00000008 [3]     : GPIO3 (0)
    // 0x00000004 [2]     : GPIO2 (0)
    // 0x00000002 [1]     : GPIO1 (0)
    // 0x00000001 [0]     : GPIO0 (0)
    io_ro_32 irqsummary_proc0_nonsecure[2];

    _REG_(IO_BANK0_IRQSUMMARY_PROC1_SECURE0_OFFSET) // IO_BANK0_IRQSUMMARY_PROC1_SECURE0
    // (Description copied from array index 0 register IO_BANK0_IRQSUMMARY_PROC1_SECURE0 applies similarly to other array indexes)
    //
    // 0x80000000 [31]    : GPIO31 (0)
    // 0x40000000 [30]    : GPIO30 (0)
    // 0x20000000 [29]    : GPIO29 (0)
    // 0x10000000 [28]    : GPIO28 (0)
    // 0x08000000 [27]    : GPIO27 (0)
    // 0x04000000 [26]    : GPIO26 (0)
    // 0x02000000 [25]    : GPIO25 (0)
    // 0x01000000 [24]    : GPIO24 (0)
    // 0x00800000 [23]    : GPIO23 (0)
    // 0x00400000 [22]    : GPIO22 (0)
    // 0x00200000 [21]    : GPIO21 (0)
    // 0x00100000 [20]    : GPIO20 (0)
    // 0x00080000 [19]    : GPIO19 (0)
    // 0x00040000 [18]    : GPIO18 (0)
    // 0x00020000 [17]    : GPIO17 (0)
    // 0x00010000 [16]    : GPIO16 (0)
    // 0x00008000 [15]    : GPIO15 (0)
    // 0x00004000 [14]    : GPIO14 (0)
    // 0x00002000 [13]    : GPIO13 (0)
    // 0x00001000 [12]    : GPIO12 (0)
    // 0x00000800 [11]    : GPIO11 (0)
    // 0x00000400 [10]    : GPIO10 (0)
    // 0x00000200 [9]     : GPIO9 (0)
    // 0x00000100 [8]     : GPIO8 (0)
    // 0x00000080 [7]     : GPIO7 (0)
    // 0x00000040 [6]     : GPIO6 (0)
    // 0x00000020 [5]     : GPIO5 (0)
    // 0x00000010 [4]     : GPIO4 (0)
    // 0x00000008 [3]     : GPIO3 (0)
    // 0x00000004 [2]     : GPIO2 (0)
    // 0x00000002 [1]     : GPIO1 (0)
    // 0x00000001 [0]     : GPIO0 (0)
    io_ro_32 irqsummary_proc1_secure[2];

    _REG_(IO_BANK0_IRQSUMMARY_PROC1_NONSECURE0_OFFSET) // IO_BANK0_IRQSUMMARY_PROC1_NONSECURE0
    // (Description copied from array index 0 register IO_BANK0_IRQSUMMARY_PROC1_NONSECURE0 applies similarly to other array indexes)
    //
    // 0x80000000 [31]    : GPIO31 (0)
    // 0x40000000 [30]    : GPIO30 (0)
    // 0x20000000 [29]    : GPIO29 (0)
    // 0x10000000 [28]    : GPIO28 (0)
    // 0x08000000 [27]    : GPIO27 (0)
    // 0x04000000 [26]    : GPIO26 (0)
    // 0x02000000 [25]    : GPIO25 (0)
    // 0x01000000 [24]    : GPIO24 (0)
    // 0x00800000 [23]    : GPIO23 (0)
    // 0x00400000 [22]    : GPIO22 (0)
    // 0x00200000 [21]    : GPIO21 (0)
    // 0x00100000 [20]    : GPIO20 (0)
    // 0x00080000 [19]    : GPIO19 (0)
    // 0x00040000 [18]    : GPIO18 (0)
    // 0x00020000 [17]    : GPIO17 (0)
    // 0x00010000 [16]    : GPIO16 (0)
    // 0x00008000 [15]    : GPIO15 (0)
    // 0x00004000 [14]    : GPIO14 (0)
    // 0x00002000 [13]    : GPIO13 (0)
    // 0x00001000 [12]    : GPIO12 (0)
    // 0x00000800 [11]    : GPIO11 (0)
    // 0x00000400 [10]    : GPIO10 (0)
    // 0x00000200 [9]     : GPIO9 (0)
    // 0x00000100 [8]     : GPIO8 (0)
    // 0x00000080 [7]     : GPIO7 (0)
    // 0x00000040 [6]     : GPIO6 (0)
    // 0x00000020 [5]     : GPIO5 (0)
    // 0x00000010 [4]     : GPIO4 (0)
    // 0x00000008 [3]     : GPIO3 (0)
    // 0x00000004 [2]     : GPIO2 (0)
    // 0x00000002 [1]     : GPIO1 (0)
    // 0x00000001 [0]     : GPIO0 (0)
    io_ro_32 irqsummary_proc1_nonsecure[2];

    _REG_(IO_BANK0_IRQSUMMARY_COMA_WAKE_SECURE0_OFFSET) // IO_BANK0_IRQSUMMARY_COMA_WAKE_SECURE0
    // (Description copied from array index 0 register IO_BANK0_IRQSUMMARY_COMA_WAKE_SECURE0 applies similarly to other array indexes)
    //
    // 0x80000000 [31]    : GPIO31 (0)
    // 0x40000000 [30]    : GPIO30 (0)
    // 0x20000000 [29]    : GPIO29 (0)
    // 0x10000000 [28]    : GPIO28 (0)
    // 0x08000000 [27]    : GPIO27 (0)
    // 0x04000000 [26]    : GPIO26 (0)
    // 0x02000000 [25]    : GPIO25 (0)
    // 0x01000000 [24]    : GPIO24 (0)
    // 0x00800000 [23]    : GPIO23 (0)
    // 0x00400000 [22]    : GPIO22 (0)
    // 0x00200000 [21]    : GPIO21 (0)
    // 0x00100000 [20]    : GPIO20 (0)
    // 0x00080000 [19]    : GPIO19 (0)
    // 0x00040000 [18]    : GPIO18 (0)
    // 0x00020000 [17]    : GPIO17 (0)
    // 0x00010000 [16]    : GPIO16 (0)
    // 0x00008000 [15]    : GPIO15 (0)
    // 0x00004000 [14]    : GPIO14 (0)
    // 0x00002000 [13]    : GPIO13 (0)
    // 0x00001000 [12]    : GPIO12 (0)
    // 0x00000800 [11]    : GPIO11 (0)
    // 0x00000400 [10]    : GPIO10 (0)
    // 0x00000200 [9]     : GPIO9 (0)
    // 0x00000100 [8]     : GPIO8 (0)
    // 0x00000080 [7]     : GPIO7 (0)
    // 0x00000040 [6]     : GPIO6 (0)
    // 0x00000020 [5]     : GPIO5 (0)
    // 0x00000010 [4]     : GPIO4 (0)
    // 0x00000008 [3]     : GPIO3 (0)
    // 0x00000004 [2]     : GPIO2 (0)
    // 0x00000002 [1]     : GPIO1 (0)
    // 0x00000001 [0]     : GPIO0 (0)
    io_ro_32 irqsummary_coma_wake_secure[2];

    _REG_(IO_BANK0_IRQSUMMARY_COMA_WAKE_NONSECURE0_OFFSET) // IO_BANK0_IRQSUMMARY_COMA_WAKE_NONSECURE0
    // (Description copied from array index 0 register IO_BANK0_IRQSUMMARY_COMA_WAKE_NONSECURE0 applies similarly to other array indexes)
    //
    // 0x80000000 [31]    : GPIO31 (0)
    // 0x40000000 [30]    : GPIO30 (0)
    // 0x20000000 [29]    : GPIO29 (0)
    // 0x10000000 [28]    : GPIO28 (0)
    // 0x08000000 [27]    : GPIO27 (0)
    // 0x04000000 [26]    : GPIO26 (0)
    // 0x02000000 [25]    : GPIO25 (0)
    // 0x01000000 [24]    : GPIO24 (0)
    // 0x00800000 [23]    : GPIO23 (0)
    // 0x00400000 [22]    : GPIO22 (0)
    // 0x00200000 [21]    : GPIO21 (0)
    // 0x00100000 [20]    : GPIO20 (0)
    // 0x00080000 [19]    : GPIO19 (0)
    // 0x00040000 [18]    : GPIO18 (0)
    // 0x00020000 [17]    : GPIO17 (0)
    // 0x00010000 [16]    : GPIO16 (0)
    // 0x00008000 [15]    : GPIO15 (0)
    // 0x00004000 [14]    : GPIO14 (0)
    // 0x00002000 [13]    : GPIO13 (0)
    // 0x00001000 [12]    : GPIO12 (0)
    // 0x00000800 [11]    : GPIO11 (0)
    // 0x00000400 [10]    : GPIO10 (0)
    // 0x00000200 [9]     : GPIO9 (0)
    // 0x00000100 [8]     : GPIO8 (0)
    // 0x00000080 [7]     : GPIO7 (0)
    // 0x00000040 [6]     : GPIO6 (0)
    // 0x00000020 [5]     : GPIO5 (0)
    // 0x00000010 [4]     : GPIO4 (0)
    // 0x00000008 [3]     : GPIO3 (0)
    // 0x00000004 [2]     : GPIO2 (0)
    // 0x00000002 [1]     : GPIO1 (0)
    // 0x00000001 [0]     : GPIO0 (0)
    io_ro_32 irqsummary_coma_wake_nonsecure[2];

    _REG_(IO_BANK0_INTR0_OFFSET) // IO_BANK0_INTR0
    // (Description copied from array index 0 register IO_BANK0_INTR0 applies similarly to other array indexes)
    //
    // Raw Interrupts
    // 0x80000000 [31]    : GPIO7_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO7_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO7_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO7_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO6_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO6_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO6_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO6_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO5_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO5_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO5_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO5_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO4_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO4_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO4_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO4_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO3_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO3_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO3_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO3_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO2_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO2_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO2_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO2_LEVEL_LOW (0)
    // 0x00000080 [7]     : GPIO1_EDGE_HIGH (0)
    // 0x00000040 [6]     : GPIO1_EDGE_LOW (0)
    // 0x00000020 [5]     : GPIO1_LEVEL_HIGH (0)
    // 0x00000010 [4]     : GPIO1_LEVEL_LOW (0)
    // 0x00000008 [3]     : GPIO0_EDGE_HIGH (0)
    // 0x00000004 [2]     : GPIO0_EDGE_LOW (0)
    // 0x00000002 [1]     : GPIO0_LEVEL_HIGH (0)
    // 0x00000001 [0]     : GPIO0_LEVEL_LOW (0)
    io_rw_32 intr[6];

    io_irq_ctrl_hw_t proc0_irq_ctrl;

    io_irq_ctrl_hw_t proc1_irq_ctrl;

    io_irq_ctrl_hw_t dormant_wake_irq_ctrl;
} iobank0_hw_t;

#define iobank0_hw ((iobank0_hw_t *)IO_BANK0_BASE)
/// \end::iobank0_hw[]

static_assert( NUM_BANK0_GPIOS == 48, "");

#endif
