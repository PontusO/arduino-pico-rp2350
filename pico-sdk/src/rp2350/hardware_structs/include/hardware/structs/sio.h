// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/*
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_STRUCTS_SIO_H
#define _HARDWARE_STRUCTS_SIO_H

#include "hardware/address_mapped.h"
#include "hardware/regs/sio.h"
#include "hardware/structs/interp.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sio
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sio.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE]: FIELDNAME (RESETVALUE): DESCRIPTION

typedef struct {
    _REG_(SIO_CPUID_OFFSET) // SIO_CPUID
    // Processor core identifier
    io_ro_32 cpuid;

    _REG_(SIO_GPIO_IN_OFFSET) // SIO_GPIO_IN
    // Input value for GPIO0
    io_ro_32 gpio_in;

    _REG_(SIO_GPIO_HI_IN_OFFSET) // SIO_GPIO_HI_IN
    // Input value on GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0): Input value on QSPI SD0 (MOSI), SD1 (MISO), SD2 and SD3 pins
    // 0x08000000 [27]    : QSPI_CSN (0): Input value on QSPI CSn pin
    // 0x04000000 [26]    : QSPI_SCK (0): Input value on QSPI SCK pin
    // 0x02000000 [25]    : USB_DM (0): Input value on USB D- pin
    // 0x01000000 [24]    : USB_DP (0): Input value on USB D+ pin
    // 0x0000ffff [15:0]  : GPIO (0): Input value on GPIO32
    io_ro_32 gpio_hi_in;

    uint32_t _pad0;

    _REG_(SIO_GPIO_OUT_OFFSET) // SIO_GPIO_OUT
    // GPIO0
    io_rw_32 gpio_out;

    _REG_(SIO_GPIO_HI_OUT_OFFSET) // SIO_GPIO_HI_OUT
    // Output value for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0): Output value for QSPI SD0 (MOSI), SD1 (MISO), SD2 and SD3 pins
    // 0x08000000 [27]    : QSPI_CSN (0): Output value for QSPI CSn pin
    // 0x04000000 [26]    : QSPI_SCK (0): Output value for QSPI SCK pin
    // 0x02000000 [25]    : USB_DM (0): Output value for USB D- pin
    // 0x01000000 [24]    : USB_DP (0): Output value for USB D+ pin
    // 0x0000ffff [15:0]  : GPIO (0): Output value for GPIO32
    io_rw_32 gpio_hi_out;

    _REG_(SIO_GPIO_OUT_SET_OFFSET) // SIO_GPIO_OUT_SET
    // GPIO0
    io_wo_32 gpio_set;

    _REG_(SIO_GPIO_HI_OUT_SET_OFFSET) // SIO_GPIO_HI_OUT_SET
    // Output value set for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0)
    // 0x08000000 [27]    : QSPI_CSN (0)
    // 0x04000000 [26]    : QSPI_SCK (0)
    // 0x02000000 [25]    : USB_DM (0)
    // 0x01000000 [24]    : USB_DP (0)
    // 0x0000ffff [15:0]  : GPIO (0)
    io_wo_32 gpio_hi_set;

    _REG_(SIO_GPIO_OUT_CLR_OFFSET) // SIO_GPIO_OUT_CLR
    // GPIO0
    io_wo_32 gpio_clr;

    _REG_(SIO_GPIO_HI_OUT_CLR_OFFSET) // SIO_GPIO_HI_OUT_CLR
    // Output value clear for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0)
    // 0x08000000 [27]    : QSPI_CSN (0)
    // 0x04000000 [26]    : QSPI_SCK (0)
    // 0x02000000 [25]    : USB_DM (0)
    // 0x01000000 [24]    : USB_DP (0)
    // 0x0000ffff [15:0]  : GPIO (0)
    io_wo_32 gpio_hi_clr;

    _REG_(SIO_GPIO_OUT_XOR_OFFSET) // SIO_GPIO_OUT_XOR
    // GPIO0
    io_wo_32 gpio_togl;

    _REG_(SIO_GPIO_HI_OUT_XOR_OFFSET) // SIO_GPIO_HI_OUT_XOR
    // Output value XOR for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0)
    // 0x08000000 [27]    : QSPI_CSN (0)
    // 0x04000000 [26]    : QSPI_SCK (0)
    // 0x02000000 [25]    : USB_DM (0)
    // 0x01000000 [24]    : USB_DP (0)
    // 0x0000ffff [15:0]  : GPIO (0)
    io_wo_32 gpio_hi_togl;

    _REG_(SIO_GPIO_OE_OFFSET) // SIO_GPIO_OE
    // GPIO0
    io_rw_32 gpio_oe;

    _REG_(SIO_GPIO_HI_OE_OFFSET) // SIO_GPIO_HI_OE
    // Output enable value for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0): Output enable value for QSPI SD0 (MOSI), SD1 (MISO), SD2 and SD3 pins
    // 0x08000000 [27]    : QSPI_CSN (0): Output enable value for QSPI CSn pin
    // 0x04000000 [26]    : QSPI_SCK (0): Output enable value for QSPI SCK pin
    // 0x02000000 [25]    : USB_DM (0): Output enable value for USB D- pin
    // 0x01000000 [24]    : USB_DP (0): Output enable value for USB D+ pin
    // 0x0000ffff [15:0]  : GPIO (0): Output enable value for GPIO32
    io_rw_32 gpio_hi_oe;

    _REG_(SIO_GPIO_OE_SET_OFFSET) // SIO_GPIO_OE_SET
    // GPIO0
    io_wo_32 gpio_oe_set;

    _REG_(SIO_GPIO_HI_OE_SET_OFFSET) // SIO_GPIO_HI_OE_SET
    // Output enable set for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0)
    // 0x08000000 [27]    : QSPI_CSN (0)
    // 0x04000000 [26]    : QSPI_SCK (0)
    // 0x02000000 [25]    : USB_DM (0)
    // 0x01000000 [24]    : USB_DP (0)
    // 0x0000ffff [15:0]  : GPIO (0)
    io_wo_32 gpio_hi_oe_set;

    _REG_(SIO_GPIO_OE_CLR_OFFSET) // SIO_GPIO_OE_CLR
    // GPIO0
    io_wo_32 gpio_oe_clr;

    _REG_(SIO_GPIO_HI_OE_CLR_OFFSET) // SIO_GPIO_HI_OE_CLR
    // Output enable clear for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0)
    // 0x08000000 [27]    : QSPI_CSN (0)
    // 0x04000000 [26]    : QSPI_SCK (0)
    // 0x02000000 [25]    : USB_DM (0)
    // 0x01000000 [24]    : USB_DP (0)
    // 0x0000ffff [15:0]  : GPIO (0)
    io_wo_32 gpio_hi_oe_clr;

    _REG_(SIO_GPIO_OE_XOR_OFFSET) // SIO_GPIO_OE_XOR
    // GPIO0
    io_wo_32 gpio_oe_togl;

    _REG_(SIO_GPIO_HI_OE_XOR_OFFSET) // SIO_GPIO_HI_OE_XOR
    // Output enable XOR for GPIO32
    // 0xf0000000 [31:28] : QSPI_SD (0)
    // 0x08000000 [27]    : QSPI_CSN (0)
    // 0x04000000 [26]    : QSPI_SCK (0)
    // 0x02000000 [25]    : USB_DM (0)
    // 0x01000000 [24]    : USB_DP (0)
    // 0x0000ffff [15:0]  : GPIO (0)
    io_wo_32 gpio_hi_oe_togl;

    _REG_(SIO_FIFO_ST_OFFSET) // SIO_FIFO_ST
    // Status register for inter-core FIFOs (mailboxes)
    // 0x00000008 [3]     : ROE (0): Sticky flag indicating the RX FIFO was read when empty
    // 0x00000004 [2]     : WOF (0): Sticky flag indicating the TX FIFO was written when full
    // 0x00000002 [1]     : RDY (1): Value is 1 if this core's TX FIFO is not full (i
    // 0x00000001 [0]     : VLD (0): Value is 1 if this core's RX FIFO is not empty (i
    io_rw_32 fifo_st;

    _REG_(SIO_FIFO_WR_OFFSET) // SIO_FIFO_WR
    // Write access to this core's TX FIFO
    io_wo_32 fifo_wr;

    _REG_(SIO_FIFO_RD_OFFSET) // SIO_FIFO_RD
    // Read access to this core's RX FIFO
    io_ro_32 fifo_rd;

    _REG_(SIO_SPINLOCK_ST_OFFSET) // SIO_SPINLOCK_ST
    // Spinlock state
    io_ro_32 spinlock_st;

    uint32_t _pad1[8];

    interp_hw_t interp[2];

    _REG_(SIO_SPINLOCK0_OFFSET) // SIO_SPINLOCK0
    // (Description copied from array index 0 register SIO_SPINLOCK0 applies similarly to other array indexes)
    //
    // Reading from a spinlock address will:
    io_rw_32 spinlock[32];

    _REG_(SIO_DOORBELL_OUT_SET_OFFSET) // SIO_DOORBELL_OUT_SET
    // Trigger a doorbell interrupt on the opposite core
    // 0x000000ff [7:0]   : DOORBELL_OUT_SET (0)
    io_rw_32 doorbell_out_set;

    _REG_(SIO_DOORBELL_OUT_CLR_OFFSET) // SIO_DOORBELL_OUT_CLR
    // Clear doorbells which have been posted to the opposite core
    // 0x000000ff [7:0]   : DOORBELL_OUT_CLR (0)
    io_rw_32 doorbell_out_clr;

    _REG_(SIO_DOORBELL_IN_SET_OFFSET) // SIO_DOORBELL_IN_SET
    // Write 1s to trigger doorbell interrupts on this core
    // 0x000000ff [7:0]   : DOORBELL_IN_SET (0)
    io_rw_32 doorbell_in_set;

    _REG_(SIO_DOORBELL_IN_CLR_OFFSET) // SIO_DOORBELL_IN_CLR
    // Check and acknowledge doorbells posted to this core
    // 0x000000ff [7:0]   : DOORBELL_IN_CLR (0)
    io_rw_32 doorbell_in_clr;

    _REG_(SIO_PERI_NONSEC_OFFSET) // SIO_PERI_NONSEC
    // Detach certain core-local peripherals from Secure SIO, and attach them to NonSecure SIO, so that NonSecure software...
    // 0x00000020 [5]     : TMDS (0): IF 1, detach TMDS encoder (of this core) from the Secure SIO, and attach to the NonSecure SIO
    // 0x00000002 [1]     : INTERP1 (0): If 1, detach interpolator 1 (of this core) from the Secure SIO, and attach to the...
    // 0x00000001 [0]     : INTERP0 (0): If 1, detach interpolator 0 (of this core) from the Secure SIO, and attach to the...
    io_rw_32 peri_nonsec;

    uint32_t _pad2[3];

    _REG_(SIO_RISCV_SOFTIRQ_OFFSET) // SIO_RISCV_SOFTIRQ
    // Control the assertion of the standard software interrupt (MIP
    // 0x00000200 [9]     : CORE1_CLR (0): Write 1 to atomically clear the core 1 software interrupt flag
    // 0x00000100 [8]     : CORE0_CLR (0): Write 1 to atomically clear the core 0 software interrupt flag
    // 0x00000002 [1]     : CORE1_SET (0): Write 1 to atomically set the core 1 software interrupt flag
    // 0x00000001 [0]     : CORE0_SET (0): Write 1 to atomically set the core 0 software interrupt flag
    io_rw_32 riscv_softirq;

    _REG_(SIO_MTIME_CTRL_OFFSET) // SIO_MTIME_CTRL
    // Control register for the RISC-V 64-bit Machine-mode timer
    // 0x00000008 [3]     : DBGPAUSE_CORE1 (1): If 1, the timer pauses when core 1 is in the debug halt state
    // 0x00000004 [2]     : DBGPAUSE_CORE0 (1): If 1, the timer pauses when core 0 is in the debug halt state
    // 0x00000002 [1]     : FULLSPEED (0): If 1, increment the timer every cycle (i
    // 0x00000001 [0]     : EN (1): Timer enable bit
    io_rw_32 mtime_ctrl;

    uint32_t _pad3[2];

    _REG_(SIO_MTIME_OFFSET) // SIO_MTIME
    // Read/write access to the high half of RISC-V Machine-mode timer
    io_rw_32 mtime;

    _REG_(SIO_MTIMEH_OFFSET) // SIO_MTIMEH
    // Read/write access to the high half of RISC-V Machine-mode timer
    io_rw_32 mtimeh;

    _REG_(SIO_MTIMECMP_OFFSET) // SIO_MTIMECMP
    // Low half of RISC-V Machine-mode timer comparator
    io_rw_32 mtimecmp;

    _REG_(SIO_MTIMECMPH_OFFSET) // SIO_MTIMECMPH
    // High half of RISC-V Machine-mode timer comparator
    io_rw_32 mtimecmph;

    _REG_(SIO_TMDS_CTRL_OFFSET) // SIO_TMDS_CTRL
    // Control register for TMDS encoder
    // 0x10000000 [28]    : CLEAR_BALANCE (0): Clear the running DC balance state of the TMDS encoders
    // 0x08000000 [27]    : PIX2_NOSHIFT (0): When encoding two pixels's worth of symbols in one cycle (a read of a...
    // 0x07000000 [26:24] : PIX_SHIFT (0): Shift applied to the colour data register with each read of a POP alias register
    // 0x00800000 [23]    : INTERLEAVE (0): Enable lane interleaving for reads of PEEK_SINGLE/POP_SINGLE
    // 0x001c0000 [20:18] : L2_NBITS (0): Number of valid colour MSBs for lane 2 (1-8 bits, encoded as 0 through 7)
    // 0x00038000 [17:15] : L1_NBITS (0): Number of valid colour MSBs for lane 1 (1-8 bits, encoded as 0 through 7)
    // 0x00007000 [14:12] : L0_NBITS (0): Number of valid colour MSBs for lane 0 (1-8 bits, encoded as 0 through 7)
    // 0x00000f00 [11:8]  : L2_ROT (0): Right-rotate the 16 LSBs of the colour accumulator by 0-15 bits, in order to get...
    // 0x000000f0 [7:4]   : L1_ROT (0): Right-rotate the 16 LSBs of the colour accumulator by 0-15 bits, in order to get...
    // 0x0000000f [3:0]   : L0_ROT (0): Right-rotate the 16 LSBs of the colour accumulator by 0-15 bits, in order to get...
    io_rw_32 tmds_ctrl;

    _REG_(SIO_TMDS_WDATA_OFFSET) // SIO_TMDS_WDATA
    // Write-only access to the TMDS colour data register
    io_wo_32 tmds_wdata;

    _REG_(SIO_TMDS_PEEK_SINGLE_OFFSET) // SIO_TMDS_PEEK_SINGLE
    // Get the encoding of one pixel's worth of colour data, packed into a 32-bit value (3x10-bit symbols)
    io_ro_32 tmds_peek_single;

    _REG_(SIO_TMDS_POP_SINGLE_OFFSET) // SIO_TMDS_POP_SINGLE
    // Get the encoding of one pixel's worth of colour data, packed into a 32-bit value
    io_ro_32 tmds_pop_single;

    _REG_(SIO_TMDS_PEEK_DOUBLE_L0_OFFSET) // SIO_TMDS_PEEK_DOUBLE_L0
    // Get lane 0 of the encoding of two pixels' worth of colour data
    io_ro_32 tmds_peek_double_l0;

    _REG_(SIO_TMDS_POP_DOUBLE_L0_OFFSET) // SIO_TMDS_POP_DOUBLE_L0
    // Get lane 0 of the encoding of two pixels' worth of colour data
    io_ro_32 tmds_pop_double_l0;

    _REG_(SIO_TMDS_PEEK_DOUBLE_L1_OFFSET) // SIO_TMDS_PEEK_DOUBLE_L1
    // Get lane 1 of the encoding of two pixels' worth of colour data
    io_ro_32 tmds_peek_double_l1;

    _REG_(SIO_TMDS_POP_DOUBLE_L1_OFFSET) // SIO_TMDS_POP_DOUBLE_L1
    // Get lane 1 of the encoding of two pixels' worth of colour data
    io_ro_32 tmds_pop_double_l1;

    _REG_(SIO_TMDS_PEEK_DOUBLE_L2_OFFSET) // SIO_TMDS_PEEK_DOUBLE_L2
    // Get lane 2 of the encoding of two pixels' worth of colour data
    io_ro_32 tmds_peek_double_l2;

    _REG_(SIO_TMDS_POP_DOUBLE_L2_OFFSET) // SIO_TMDS_POP_DOUBLE_L2
    // Get lane 2 of the encoding of two pixels' worth of colour data
    io_ro_32 tmds_pop_double_l2;
} sio_hw_t;

#define sio_hw ((sio_hw_t *)SIO_BASE)

// Note this alias is for accessing NS SIO from Secure code -- NS code
// should just use the regular sio_hw define:
#define sio_ns_hw ((sio_hw_t *)SIO_NONSEC_BASE)

#endif
