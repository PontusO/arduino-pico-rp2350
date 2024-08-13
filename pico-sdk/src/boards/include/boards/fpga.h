/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------

#ifndef _BOARDS_FPGA_H
#define _BOARDS_FPGA_H

#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#endif
#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 28
#endif

#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 29
#endif

#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 0
#endif

#define PICO_FLASH_SPI_CLKDIV 4

// This is true for the W25Q40 on first-run picoboards:
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (16 * 1024 * 1024)
#endif

#define PICO_ON_FPGA 1

#define PICO_SVIDEO_COLOR_PIN_COUNT 15
#define PICO_SVIDEO_DPI_PIXEL_RSHIFT 0
#define PICO_SVIDEO_DPI_PIXEL_GSHIFT 5
#define PICO_SVIDEO_DPI_PIXEL_BSHIFT 10
#define PICO_SVIDEO_DPI_ALPHA_PIN 15

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#endif
