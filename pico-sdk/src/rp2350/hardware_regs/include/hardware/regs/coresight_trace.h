/**
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : CORESIGHT_TRACE
// Version        : 1
// Bus type       : ahbl
// Description    : Coresight block - RP specific registers
// =============================================================================
#ifndef HARDWARE_REGS_CORESIGHT_TRACE_DEFINED
#define HARDWARE_REGS_CORESIGHT_TRACE_DEFINED
// =============================================================================
// Register    : CORESIGHT_TRACE_CTRL_STATUS
// Description : miscellaneous control/status bits
#define CORESIGHT_TRACE_CTRL_STATUS_OFFSET _u(0x00000000)
#define CORESIGHT_TRACE_CTRL_STATUS_BITS   _u(0x00000003)
#define CORESIGHT_TRACE_CTRL_STATUS_RESET  _u(0x00000001)
// -----------------------------------------------------------------------------
// Field       : CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW
// Description : None
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_RESET  _u(0x0)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_BITS   _u(0x00000002)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_MSB    _u(1)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_LSB    _u(1)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH
// Description : flushes trace capture fifo
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_RESET  _u(0x1)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_BITS   _u(0x00000001)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_MSB    _u(0)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_LSB    _u(0)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_ACCESS "RW"
// =============================================================================
// Register    : CORESIGHT_TRACE_TRACE_CAPTURE_FIFO
// Description : trace capture fifo
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_OFFSET _u(0x00000004)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_BITS   _u(0xffffffff)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA
// Description : captured trace data
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_RESET  _u(0x00000000)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_BITS   _u(0xffffffff)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_MSB    _u(31)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_LSB    _u(0)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_ACCESS "RF"
// =============================================================================
#endif // HARDWARE_REGS_CORESIGHT_TRACE_DEFINED
