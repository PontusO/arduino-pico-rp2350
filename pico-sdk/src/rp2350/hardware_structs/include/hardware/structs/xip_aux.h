// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_XIP_AUX_H
#define _HARDWARE_STRUCTS_XIP_AUX_H

#include "hardware/address_mapped.h"
#include "hardware/regs/xip_aux.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_xip_aux
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/xip_aux.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(XIP_AUX_STREAM_OFFSET) // XIP_AUX_STREAM
    // Read the XIP stream FIFO (fast bus access to XIP_CTRL_STREAM_FIFO)
    io_ro_32 stream;

    _REG_(XIP_AUX_QMI_DIRECT_TX_OFFSET) // XIP_AUX_QMI_DIRECT_TX
    // Write to the QMI direct-mode TX FIFO (fast bus access to QMI_DIRECT_TX)
    // 0x00100000 [20]    : NOPUSH (0): Inhibit the RX FIFO push that would correspond to this TX FIFO entry
    // 0x00080000 [19]    : OE (0): Output enable (active-high)
    // 0x00040000 [18]    : DWIDTH (0): Data width
    // 0x00030000 [17:16] : IWIDTH (0): Configure whether this FIFO record is transferred with single/dual/quad interface...
    // 0x0000ffff [15:0]  : DATA (0): Data pushed here will be clocked out falling edges of SCK (or before the very first...
    io_wo_32 qmi_direct_tx;

    _REG_(XIP_AUX_QMI_DIRECT_RX_OFFSET) // XIP_AUX_QMI_DIRECT_RX
    // Read from the QMI direct-mode RX FIFO (fast bus access to QMI_DIRECT_RX)
    // 0x0000ffff [15:0]  : QMI_DIRECT_RX (0): With each byte clocked out on the serial interface, one byte will...
    io_ro_32 qmi_direct_rx;
} xip_aux_hw_t;

#define xip_aux_hw ((xip_aux_hw_t *)XIP_AUX_BASE)

#endif
