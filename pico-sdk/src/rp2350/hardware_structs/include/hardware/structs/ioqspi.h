// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_IOQSPI_H
#define _HARDWARE_STRUCTS_IOQSPI_H

#include "hardware/address_mapped.h"
#include "hardware/regs/io_qspi.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_io_qspi
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/io_qspi.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(IO_QSPI_GPIO_QSPI_SCLK_STATUS_OFFSET) // IO_QSPI_GPIO_QSPI_SCLK_STATUS
    // 0x04000000 [26]    : IRQTOPROC (0): interrupt to processors, after override is applied
    // 0x00020000 [17]    : INFROMPAD (0): input signal from pad, before filtering and override are applied
    // 0x00002000 [13]    : OETOPAD (0): output enable to pad after register overide is applied
    // 0x00000200 [9]     : OUTTOPAD (0): output signal to pad after register overide is applied
    io_ro_32 status;

    _REG_(IO_QSPI_GPIO_QSPI_SCLK_CTRL_OFFSET) // IO_QSPI_GPIO_QSPI_SCLK_CTRL
    // 0x30000000 [29:28] : IRQOVER (0)
    // 0x00030000 [17:16] : INOVER (0)
    // 0x0000c000 [15:14] : OEOVER (0)
    // 0x00003000 [13:12] : OUTOVER (0)
    // 0x0000001f [4:0]   : FUNCSEL (0x1f): 0-31 -> selects pin function according to the gpio table
    io_rw_32 ctrl;
} ioqspi_status_ctrl_hw_t;

typedef struct {
    _REG_(IO_QSPI_PROC0_INTE_OFFSET) // IO_QSPI_PROC0_INTE
    // Interrupt Enable for proc0
    // 0x80000000 [31]    : GPIO_QSPI_SD3_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO_QSPI_SD3_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO_QSPI_SD3_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO_QSPI_SD3_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO_QSPI_SD2_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO_QSPI_SD2_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO_QSPI_SD2_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO_QSPI_SD2_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO_QSPI_SD1_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO_QSPI_SD1_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO_QSPI_SD1_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO_QSPI_SD1_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO_QSPI_SD0_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO_QSPI_SD0_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO_QSPI_SD0_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO_QSPI_SD0_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO_QSPI_SS_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO_QSPI_SS_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO_QSPI_SS_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO_QSPI_SS_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO_QSPI_SCLK_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO_QSPI_SCLK_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO_QSPI_SCLK_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO_QSPI_SCLK_LEVEL_LOW (0)
    // 0x00000080 [7]     : USBPHY_DM_EDGE_HIGH (0)
    // 0x00000040 [6]     : USBPHY_DM_EDGE_LOW (0)
    // 0x00000020 [5]     : USBPHY_DM_LEVEL_HIGH (0)
    // 0x00000010 [4]     : USBPHY_DM_LEVEL_LOW (0)
    // 0x00000008 [3]     : USBPHY_DP_EDGE_HIGH (0)
    // 0x00000004 [2]     : USBPHY_DP_EDGE_LOW (0)
    // 0x00000002 [1]     : USBPHY_DP_LEVEL_HIGH (0)
    // 0x00000001 [0]     : USBPHY_DP_LEVEL_LOW (0)
    io_rw_32 inte;

    _REG_(IO_QSPI_PROC0_INTF_OFFSET) // IO_QSPI_PROC0_INTF
    // Interrupt Force for proc0
    // 0x80000000 [31]    : GPIO_QSPI_SD3_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO_QSPI_SD3_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO_QSPI_SD3_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO_QSPI_SD3_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO_QSPI_SD2_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO_QSPI_SD2_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO_QSPI_SD2_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO_QSPI_SD2_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO_QSPI_SD1_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO_QSPI_SD1_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO_QSPI_SD1_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO_QSPI_SD1_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO_QSPI_SD0_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO_QSPI_SD0_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO_QSPI_SD0_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO_QSPI_SD0_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO_QSPI_SS_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO_QSPI_SS_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO_QSPI_SS_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO_QSPI_SS_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO_QSPI_SCLK_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO_QSPI_SCLK_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO_QSPI_SCLK_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO_QSPI_SCLK_LEVEL_LOW (0)
    // 0x00000080 [7]     : USBPHY_DM_EDGE_HIGH (0)
    // 0x00000040 [6]     : USBPHY_DM_EDGE_LOW (0)
    // 0x00000020 [5]     : USBPHY_DM_LEVEL_HIGH (0)
    // 0x00000010 [4]     : USBPHY_DM_LEVEL_LOW (0)
    // 0x00000008 [3]     : USBPHY_DP_EDGE_HIGH (0)
    // 0x00000004 [2]     : USBPHY_DP_EDGE_LOW (0)
    // 0x00000002 [1]     : USBPHY_DP_LEVEL_HIGH (0)
    // 0x00000001 [0]     : USBPHY_DP_LEVEL_LOW (0)
    io_rw_32 intf;

    _REG_(IO_QSPI_PROC0_INTS_OFFSET) // IO_QSPI_PROC0_INTS
    // Interrupt status after masking & forcing for proc0
    // 0x80000000 [31]    : GPIO_QSPI_SD3_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO_QSPI_SD3_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO_QSPI_SD3_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO_QSPI_SD3_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO_QSPI_SD2_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO_QSPI_SD2_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO_QSPI_SD2_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO_QSPI_SD2_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO_QSPI_SD1_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO_QSPI_SD1_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO_QSPI_SD1_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO_QSPI_SD1_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO_QSPI_SD0_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO_QSPI_SD0_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO_QSPI_SD0_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO_QSPI_SD0_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO_QSPI_SS_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO_QSPI_SS_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO_QSPI_SS_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO_QSPI_SS_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO_QSPI_SCLK_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO_QSPI_SCLK_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO_QSPI_SCLK_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO_QSPI_SCLK_LEVEL_LOW (0)
    // 0x00000080 [7]     : USBPHY_DM_EDGE_HIGH (0)
    // 0x00000040 [6]     : USBPHY_DM_EDGE_LOW (0)
    // 0x00000020 [5]     : USBPHY_DM_LEVEL_HIGH (0)
    // 0x00000010 [4]     : USBPHY_DM_LEVEL_LOW (0)
    // 0x00000008 [3]     : USBPHY_DP_EDGE_HIGH (0)
    // 0x00000004 [2]     : USBPHY_DP_EDGE_LOW (0)
    // 0x00000002 [1]     : USBPHY_DP_LEVEL_HIGH (0)
    // 0x00000001 [0]     : USBPHY_DP_LEVEL_LOW (0)
    io_ro_32 ints;
} io_qspi_ctrl_hw_t;

typedef struct {
    _REG_(IO_QSPI_USBPHY_DP_STATUS_OFFSET) // IO_QSPI_USBPHY_DP_STATUS
    // 0x04000000 [26]    : IRQTOPROC (0): interrupt to processors, after override is applied
    // 0x00020000 [17]    : INFROMPAD (0): input signal from pad, before filtering and override are applied
    // 0x00002000 [13]    : OETOPAD (0): output enable to pad after register overide is applied
    // 0x00000200 [9]     : OUTTOPAD (0): output signal to pad after register overide is applied
    io_ro_32 usbphy_dp_status;

    _REG_(IO_QSPI_USBPHY_DP_CTRL_OFFSET) // IO_QSPI_USBPHY_DP_CTRL
    // 0x30000000 [29:28] : IRQOVER (0)
    // 0x00030000 [17:16] : INOVER (0)
    // 0x0000c000 [15:14] : OEOVER (0)
    // 0x00003000 [13:12] : OUTOVER (0)
    // 0x0000001f [4:0]   : FUNCSEL (0x1f): 0-31 -> selects pin function according to the gpio table
    io_rw_32 usbphy_dp_ctrl;

    _REG_(IO_QSPI_USBPHY_DM_STATUS_OFFSET) // IO_QSPI_USBPHY_DM_STATUS
    // 0x04000000 [26]    : IRQTOPROC (0): interrupt to processors, after override is applied
    // 0x00020000 [17]    : INFROMPAD (0): input signal from pad, before filtering and override are applied
    // 0x00002000 [13]    : OETOPAD (0): output enable to pad after register overide is applied
    // 0x00000200 [9]     : OUTTOPAD (0): output signal to pad after register overide is applied
    io_ro_32 usbphy_dm_status;

    _REG_(IO_QSPI_USBPHY_DM_CTRL_OFFSET) // IO_QSPI_USBPHY_DM_CTRL
    // 0x30000000 [29:28] : IRQOVER (0)
    // 0x00030000 [17:16] : INOVER (0)
    // 0x0000c000 [15:14] : OEOVER (0)
    // 0x00003000 [13:12] : OUTOVER (0)
    // 0x0000001f [4:0]   : FUNCSEL (0x1f): 0-31 -> selects pin function according to the gpio table
    io_rw_32 usbphy_dm_ctrl;

    ioqspi_status_ctrl_hw_t io[NUM_QSPI_GPIOS]; // 6

    uint32_t _pad0[112];

    _REG_(IO_QSPI_IRQSUMMARY_PROC0_SECURE_OFFSET) // IO_QSPI_IRQSUMMARY_PROC0_SECURE
    // 0x00000080 [7]     : GPIO_QSPI_SD3 (0)
    // 0x00000040 [6]     : GPIO_QSPI_SD2 (0)
    // 0x00000020 [5]     : GPIO_QSPI_SD1 (0)
    // 0x00000010 [4]     : GPIO_QSPI_SD0 (0)
    // 0x00000008 [3]     : GPIO_QSPI_SS (0)
    // 0x00000004 [2]     : GPIO_QSPI_SCLK (0)
    // 0x00000002 [1]     : USBPHY_DM (0)
    // 0x00000001 [0]     : USBPHY_DP (0)
    io_ro_32 irqsummary_proc0_secure;

    _REG_(IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_OFFSET) // IO_QSPI_IRQSUMMARY_PROC0_NONSECURE
    // 0x00000080 [7]     : GPIO_QSPI_SD3 (0)
    // 0x00000040 [6]     : GPIO_QSPI_SD2 (0)
    // 0x00000020 [5]     : GPIO_QSPI_SD1 (0)
    // 0x00000010 [4]     : GPIO_QSPI_SD0 (0)
    // 0x00000008 [3]     : GPIO_QSPI_SS (0)
    // 0x00000004 [2]     : GPIO_QSPI_SCLK (0)
    // 0x00000002 [1]     : USBPHY_DM (0)
    // 0x00000001 [0]     : USBPHY_DP (0)
    io_ro_32 irqsummary_proc0_nonsecure;

    _REG_(IO_QSPI_IRQSUMMARY_PROC1_SECURE_OFFSET) // IO_QSPI_IRQSUMMARY_PROC1_SECURE
    // 0x00000080 [7]     : GPIO_QSPI_SD3 (0)
    // 0x00000040 [6]     : GPIO_QSPI_SD2 (0)
    // 0x00000020 [5]     : GPIO_QSPI_SD1 (0)
    // 0x00000010 [4]     : GPIO_QSPI_SD0 (0)
    // 0x00000008 [3]     : GPIO_QSPI_SS (0)
    // 0x00000004 [2]     : GPIO_QSPI_SCLK (0)
    // 0x00000002 [1]     : USBPHY_DM (0)
    // 0x00000001 [0]     : USBPHY_DP (0)
    io_ro_32 irqsummary_proc1_secure;

    _REG_(IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_OFFSET) // IO_QSPI_IRQSUMMARY_PROC1_NONSECURE
    // 0x00000080 [7]     : GPIO_QSPI_SD3 (0)
    // 0x00000040 [6]     : GPIO_QSPI_SD2 (0)
    // 0x00000020 [5]     : GPIO_QSPI_SD1 (0)
    // 0x00000010 [4]     : GPIO_QSPI_SD0 (0)
    // 0x00000008 [3]     : GPIO_QSPI_SS (0)
    // 0x00000004 [2]     : GPIO_QSPI_SCLK (0)
    // 0x00000002 [1]     : USBPHY_DM (0)
    // 0x00000001 [0]     : USBPHY_DP (0)
    io_ro_32 irqsummary_proc1_nonsecure;

    _REG_(IO_QSPI_IRQSUMMARY_COMA_WAKE_SECURE_OFFSET) // IO_QSPI_IRQSUMMARY_COMA_WAKE_SECURE
    // 0x00000080 [7]     : GPIO_QSPI_SD3 (0)
    // 0x00000040 [6]     : GPIO_QSPI_SD2 (0)
    // 0x00000020 [5]     : GPIO_QSPI_SD1 (0)
    // 0x00000010 [4]     : GPIO_QSPI_SD0 (0)
    // 0x00000008 [3]     : GPIO_QSPI_SS (0)
    // 0x00000004 [2]     : GPIO_QSPI_SCLK (0)
    // 0x00000002 [1]     : USBPHY_DM (0)
    // 0x00000001 [0]     : USBPHY_DP (0)
    io_ro_32 irqsummary_coma_wake_secure;

    _REG_(IO_QSPI_IRQSUMMARY_COMA_WAKE_NONSECURE_OFFSET) // IO_QSPI_IRQSUMMARY_COMA_WAKE_NONSECURE
    // 0x00000080 [7]     : GPIO_QSPI_SD3 (0)
    // 0x00000040 [6]     : GPIO_QSPI_SD2 (0)
    // 0x00000020 [5]     : GPIO_QSPI_SD1 (0)
    // 0x00000010 [4]     : GPIO_QSPI_SD0 (0)
    // 0x00000008 [3]     : GPIO_QSPI_SS (0)
    // 0x00000004 [2]     : GPIO_QSPI_SCLK (0)
    // 0x00000002 [1]     : USBPHY_DM (0)
    // 0x00000001 [0]     : USBPHY_DP (0)
    io_ro_32 irqsummary_coma_wake_nonsecure;

    _REG_(IO_QSPI_INTR_OFFSET) // IO_QSPI_INTR
    // Raw Interrupts
    // 0x80000000 [31]    : GPIO_QSPI_SD3_EDGE_HIGH (0)
    // 0x40000000 [30]    : GPIO_QSPI_SD3_EDGE_LOW (0)
    // 0x20000000 [29]    : GPIO_QSPI_SD3_LEVEL_HIGH (0)
    // 0x10000000 [28]    : GPIO_QSPI_SD3_LEVEL_LOW (0)
    // 0x08000000 [27]    : GPIO_QSPI_SD2_EDGE_HIGH (0)
    // 0x04000000 [26]    : GPIO_QSPI_SD2_EDGE_LOW (0)
    // 0x02000000 [25]    : GPIO_QSPI_SD2_LEVEL_HIGH (0)
    // 0x01000000 [24]    : GPIO_QSPI_SD2_LEVEL_LOW (0)
    // 0x00800000 [23]    : GPIO_QSPI_SD1_EDGE_HIGH (0)
    // 0x00400000 [22]    : GPIO_QSPI_SD1_EDGE_LOW (0)
    // 0x00200000 [21]    : GPIO_QSPI_SD1_LEVEL_HIGH (0)
    // 0x00100000 [20]    : GPIO_QSPI_SD1_LEVEL_LOW (0)
    // 0x00080000 [19]    : GPIO_QSPI_SD0_EDGE_HIGH (0)
    // 0x00040000 [18]    : GPIO_QSPI_SD0_EDGE_LOW (0)
    // 0x00020000 [17]    : GPIO_QSPI_SD0_LEVEL_HIGH (0)
    // 0x00010000 [16]    : GPIO_QSPI_SD0_LEVEL_LOW (0)
    // 0x00008000 [15]    : GPIO_QSPI_SS_EDGE_HIGH (0)
    // 0x00004000 [14]    : GPIO_QSPI_SS_EDGE_LOW (0)
    // 0x00002000 [13]    : GPIO_QSPI_SS_LEVEL_HIGH (0)
    // 0x00001000 [12]    : GPIO_QSPI_SS_LEVEL_LOW (0)
    // 0x00000800 [11]    : GPIO_QSPI_SCLK_EDGE_HIGH (0)
    // 0x00000400 [10]    : GPIO_QSPI_SCLK_EDGE_LOW (0)
    // 0x00000200 [9]     : GPIO_QSPI_SCLK_LEVEL_HIGH (0)
    // 0x00000100 [8]     : GPIO_QSPI_SCLK_LEVEL_LOW (0)
    // 0x00000080 [7]     : USBPHY_DM_EDGE_HIGH (0)
    // 0x00000040 [6]     : USBPHY_DM_EDGE_LOW (0)
    // 0x00000020 [5]     : USBPHY_DM_LEVEL_HIGH (0)
    // 0x00000010 [4]     : USBPHY_DM_LEVEL_LOW (0)
    // 0x00000008 [3]     : USBPHY_DP_EDGE_HIGH (0)
    // 0x00000004 [2]     : USBPHY_DP_EDGE_LOW (0)
    // 0x00000002 [1]     : USBPHY_DP_LEVEL_HIGH (0)
    // 0x00000001 [0]     : USBPHY_DP_LEVEL_LOW (0)
    io_rw_32 intr;

    io_qspi_ctrl_hw_t proc0_qspi_ctrl;

    io_qspi_ctrl_hw_t proc1_qspi_ctrl;

    io_qspi_ctrl_hw_t dormant_wake_qspi_ctrl;
} ioqspi_hw_t;

#define ioqspi_hw ((ioqspi_hw_t *)IO_QSPI_BASE)

static_assert( NUM_QSPI_GPIOS == 6, "");

#endif
