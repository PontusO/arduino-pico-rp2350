// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_RESETS_H
#define _HARDWARE_STRUCTS_RESETS_H

#include "hardware/address_mapped.h"
#include "hardware/regs/resets.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_resets
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/resets.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

/// \tag::resets_hw[]
typedef struct {
    _REG_(RESETS_RESET_OFFSET) // RESETS_RESET
    // 0x10000000 [28]    : usbctrl (1)
    // 0x08000000 [27]    : uart1 (1)
    // 0x04000000 [26]    : uart0 (1)
    // 0x02000000 [25]    : trng (1)
    // 0x01000000 [24]    : timer1 (1)
    // 0x00800000 [23]    : timer0 (1)
    // 0x00400000 [22]    : tbman (1)
    // 0x00200000 [21]    : sysinfo (1)
    // 0x00100000 [20]    : syscfg (1)
    // 0x00080000 [19]    : spi1 (1)
    // 0x00040000 [18]    : spi0 (1)
    // 0x00020000 [17]    : sha256 (1)
    // 0x00010000 [16]    : pwm (1)
    // 0x00008000 [15]    : pll_usb (1)
    // 0x00004000 [14]    : pll_sys (1)
    // 0x00002000 [13]    : pio2 (1)
    // 0x00001000 [12]    : pio1 (1)
    // 0x00000800 [11]    : pio0 (1)
    // 0x00000400 [10]    : pads_qspi (1)
    // 0x00000200 [9]     : pads_bank0 (1)
    // 0x00000100 [8]     : jtag (1)
    // 0x00000080 [7]     : io_qspi (1)
    // 0x00000040 [6]     : io_bank0 (1)
    // 0x00000020 [5]     : i2c1 (1)
    // 0x00000010 [4]     : i2c0 (1)
    // 0x00000008 [3]     : hstx (1)
    // 0x00000004 [2]     : dma (1)
    // 0x00000002 [1]     : busctrl (1)
    // 0x00000001 [0]     : adc (1)
    io_rw_32 reset;

    _REG_(RESETS_WDSEL_OFFSET) // RESETS_WDSEL
    // 0x10000000 [28]    : usbctrl (0)
    // 0x08000000 [27]    : uart1 (0)
    // 0x04000000 [26]    : uart0 (0)
    // 0x02000000 [25]    : trng (0)
    // 0x01000000 [24]    : timer1 (0)
    // 0x00800000 [23]    : timer0 (0)
    // 0x00400000 [22]    : tbman (0)
    // 0x00200000 [21]    : sysinfo (0)
    // 0x00100000 [20]    : syscfg (0)
    // 0x00080000 [19]    : spi1 (0)
    // 0x00040000 [18]    : spi0 (0)
    // 0x00020000 [17]    : sha256 (0)
    // 0x00010000 [16]    : pwm (0)
    // 0x00008000 [15]    : pll_usb (0)
    // 0x00004000 [14]    : pll_sys (0)
    // 0x00002000 [13]    : pio2 (0)
    // 0x00001000 [12]    : pio1 (0)
    // 0x00000800 [11]    : pio0 (0)
    // 0x00000400 [10]    : pads_qspi (0)
    // 0x00000200 [9]     : pads_bank0 (0)
    // 0x00000100 [8]     : jtag (0)
    // 0x00000080 [7]     : io_qspi (0)
    // 0x00000040 [6]     : io_bank0 (0)
    // 0x00000020 [5]     : i2c1 (0)
    // 0x00000010 [4]     : i2c0 (0)
    // 0x00000008 [3]     : hstx (0)
    // 0x00000004 [2]     : dma (0)
    // 0x00000002 [1]     : busctrl (0)
    // 0x00000001 [0]     : adc (0)
    io_rw_32 wdsel;

    _REG_(RESETS_RESET_DONE_OFFSET) // RESETS_RESET_DONE
    // 0x10000000 [28]    : usbctrl (0)
    // 0x08000000 [27]    : uart1 (0)
    // 0x04000000 [26]    : uart0 (0)
    // 0x02000000 [25]    : trng (0)
    // 0x01000000 [24]    : timer1 (0)
    // 0x00800000 [23]    : timer0 (0)
    // 0x00400000 [22]    : tbman (0)
    // 0x00200000 [21]    : sysinfo (0)
    // 0x00100000 [20]    : syscfg (0)
    // 0x00080000 [19]    : spi1 (0)
    // 0x00040000 [18]    : spi0 (0)
    // 0x00020000 [17]    : sha256 (0)
    // 0x00010000 [16]    : pwm (0)
    // 0x00008000 [15]    : pll_usb (0)
    // 0x00004000 [14]    : pll_sys (0)
    // 0x00002000 [13]    : pio2 (0)
    // 0x00001000 [12]    : pio1 (0)
    // 0x00000800 [11]    : pio0 (0)
    // 0x00000400 [10]    : pads_qspi (0)
    // 0x00000200 [9]     : pads_bank0 (0)
    // 0x00000100 [8]     : jtag (0)
    // 0x00000080 [7]     : io_qspi (0)
    // 0x00000040 [6]     : io_bank0 (0)
    // 0x00000020 [5]     : i2c1 (0)
    // 0x00000010 [4]     : i2c0 (0)
    // 0x00000008 [3]     : hstx (0)
    // 0x00000004 [2]     : dma (0)
    // 0x00000002 [1]     : busctrl (0)
    // 0x00000001 [0]     : adc (0)
    io_ro_32 reset_done;
} resets_hw_t;

#define resets_hw ((resets_hw_t *)RESETS_BASE)
/// \end::resets_hw[]

#endif
