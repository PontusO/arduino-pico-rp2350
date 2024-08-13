/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_BOOTROM_H
#define _PICO_BOOTROM_H

#include "bootrom_constants.h"

/** \file bootrom.h
 * \defgroup pico_bootrom pico_bootrom
 * Access to functions and data in the on-device bootrom
 *
 * This header may be included by assembly code
 */

#ifndef __ASSEMBLER__
// ROM FUNCTION SIGNATURES

#if PICO_RP2040
typedef uint32_t (*rom_popcount32_fn)(uint32_t);
typedef uint32_t (*rom_reverse32_fn)(uint32_t);
typedef uint32_t (*rom_clz32_fn)(uint32_t);
typedef uint32_t (*rom_ctz32_fn)(uint32_t);
typedef uint8_t *(*rom_memset_fn)(uint8_t *, uint8_t, uint32_t);
typedef uint32_t *(*rom_memset4_fn)(uint32_t *, uint8_t, uint32_t);
typedef uint32_t *(*rom_memcpy_fn)(uint8_t *, const uint8_t *, uint32_t);
typedef uint32_t *(*rom_memcpy44_fn)(uint32_t *, const uint32_t *, uint32_t);
#endif
// todo lets set
// STATE_RESET_PERMISSIONS
// STATE_RESET_CORE0
// STATE_RESET_CORE1
typedef void (*rom_bootrom_state_reset_fn)(uint32_t flags);
typedef void __attribute__((noreturn)) (*rom_reset_usb_boot_fn)(uint32_t, uint32_t);
typedef int (*rom_reboot_fn)(uint32_t flags, uint32_t delay_ms, uint32_t p0, uint32_t p1);
typedef rom_reset_usb_boot_fn reset_usb_boot_fn; // kept for backwards compatibility
// todo amy need int return code
typedef void (*rom_connect_internal_flash_fn)(void);
typedef void (*rom_flash_exit_xip_fn)(void);
typedef void (*rom_flash_range_erase_fn)(uint32_t, size_t, uint32_t, uint8_t);
typedef void (*rom_flash_range_program_fn)(uint32_t, const uint8_t*, size_t);
typedef void (*rom_flash_flush_cache_fn)(void);
typedef void (*rom_flash_enter_cmd_xip_fn)(void);
#if !PICO_RP2040
typedef int (*rom_get_sys_info_fn)(uint32_t *out_buffer, uint32_t out_buffer_word_size, uint32_t partition_and_flags);
typedef int (*rom_get_partition_table_info_fn)(uint32_t *out_buffer, uint32_t out_buffer_word_size, uint32_t partition_and_flags);
typedef int (*rom_explicit_buy_fn)(uint8_t *buffer, uint32_t buffer_size);
typedef void* (*rom_validate_ns_buffer_fn)(const void *addr, uint32_t size, uint32_t write, uint32_t *ok);
/**
 * @return BOOTROM_OK if successful
 *         BOOTROM_ERROR_INVALID_ARG if ns_api_num is out of range
 */
typedef intptr_t (*rom_set_rom_callback_fn)(uint callback_num, bootrom_api_callback_generic_t funcptr);
typedef int (*rom_chain_image_fn)(uint8_t *workarea_base, uint32_t workarea_size, uint32_t window_base, uint32_t window_size);
typedef int (*rom_load_partition_table_fn)(uint8_t *workarea_base, uint32_t workarea_size, bool force_reload);
typedef int (*rom_pick_ab_partition_fn)(uint8_t *workarea_base, uint32_t workarea_size, uint partition_a_num, uint32_t flash_update_boot_window_base);
typedef int (*rom_get_b_partition_fn)(uint pi_a);
typedef int (*rom_get_uf2_target_partition_fn)(uint8_t *workarea_base, uint32_t workarea_size, uint32_t family_id, resident_partition_t *partition_out);
typedef int (*rom_func_otp_access_fn)(uint8_t *buf, uint32_t buf_len, otp_cmd_t cmd);
// Apply the address translation currently specified in QMI_ATRANSx ("rolling window" hardware
// translation). Need to take care using this on the boot path, as the QMI may not yet have been
// set up, but this should be suitable for translating system bus addresses into flash storage
// addresses in user callbacks. Returns all-ones for an invalid address, which is also an invalid
// flash storage address, so invalidity is propagated.
typedef intptr_t (*rom_flash_runtime_to_storage_addr_fn)(uintptr_t flash_runtime_addr);

// Perform the specified erase/program/read operation, translating addresses according to
// QMI_ATRANSx if necessary, and checking flash permissions based on the resident partition table
// and the specified effective security level. `addr` may be either a flash runtime address or a
// flash storage address, depending on the ASPACE given in `flags`.
//
// NOTE: This function does not validate the buffer for NS access. This must be validated before
// calling if the caller is reachable from a Secure Gateway.
typedef int (*rom_flash_op_fn)(cflash_flags_t flags, uintptr_t addr, uint32_t size_bytes, uint8_t *buf);

#ifndef __riscv
typedef int (*rom_set_ns_api_permission_fn)(uint ns_api_num, bool allowed);
/**
 * Note this is not strictly a C function; you must pass the function you are calling in r4
 * @return whatever the secure call returns
 *         BOOTROM_ERROR_INVALID_STATE if no secure handler has been set from the secure side
 *            via rom_set_rom_callback_fn(BOOTROM_API_CALLBACK_secure_call, ...)
 */
typedef int (*rom_func_secure_call)(uintptr_t a0, ...);
#endif

#ifdef __riscv
typedef struct {
    uint32_t *base;
    uint32_t size;
} bootrom_stack_t;
// passed in, and out.
typedef int (*rom_set_bootrom_stack_fn)(bootrom_stack_t *stack);
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Return a bootrom lookup code based on two ASCII characters
 * \ingroup pico_bootrom
 *
 * These codes are uses to lookup data or function addresses in the bootrom
 *
 * \param c1 the first character
 * \param c2 the second character
 * \return the 'code' to use in rom_func_lookup() or rom_data_lookup()
 */
static inline uint32_t rom_table_code(uint8_t c1, uint8_t c2) {
    return ROM_TABLE_CODE((uint32_t) c1, (uint32_t) c2);
}

/*!
 * \brief Lookup a bootrom function by code
 * \ingroup pico_bootrom
 * \param code the code
 * \return a pointer to the function, or NULL if the code does not match any bootrom function
 */
void *rom_func_lookup(uint32_t code);

/*!
 * \brief Lookup a bootrom address by code
 * \ingroup pico_bootrom
 * \param code the code
 * \return a pointer to the data, or NULL if the code does not match any bootrom function
 */
void *rom_data_lookup(uint32_t code);

/*!
 * \brief Helper function to lookup the addresses of multiple bootrom functions
 * \ingroup pico_bootrom
 *
 * This method looks up the 'codes' in the table, and convert each table entry to the looked up
 * function pointer, if there is a function for that code in the bootrom.
 *
 * \param table an IN/OUT array, elements are codes on input, function pointers on success.
 * \param count the number of elements in the table
 * \return true if all the codes were found, and converted to function pointers, false otherwise
 */
bool rom_funcs_lookup(uint32_t *table, unsigned int count);

// Bootrom function: rom_table_lookup
// Returns the 32 bit pointer into the ROM if found or NULL otherwise.
#if PICO_RP2040
typedef void *(*rom_table_lookup_fn)(uint16_t *table, uint32_t code);
#else

typedef void *(*rom_table_lookup_fn)(uint32_t code, uint32_t mask);
#endif

#if PICO_C_COMPILER_IS_GNU && (__GNUC__ >= 12)
// Convert a 16 bit pointer stored at the given rom address into a 32 bit pointer
__force_inline static void *rom_hword_as_ptr(uint16_t rom_address) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    return (void *)(uintptr_t)*(uint16_t *)(uintptr_t)rom_address;
#pragma GCC diagnostic pop
}
#else
// Convert a 16 bit pointer stored at the given rom address into a 32 bit pointer
#define rom_hword_as_ptr(rom_address) (void *)(uintptr_t)(*(uint16_t *)(uintptr_t)(rom_address))
#endif

// todo amy remove this ROM size check once 64k ROM is no longer a thing
#ifdef __riscv
static __force_inline bool rom_size_is_64k(void) {
    // Detect ROM size by testing for bus fault at +32k
    uint result;
    pico_default_asm_volatile (
        "li %0, 0\n"
        // Save and disable IRQs before touching trap vector
        "csrr t2, mstatus\n"
        "csrci mstatus, 0x8\n"
        // Set up trap vector to skip the instruction which sets the %0 flag
        "la t0, 1f\n"
        "csrrw t0, mtvec, t0\n"
        // This load will fault if the bootrom is no larger than 32k:
        "li t1, 32 * 1024\n"
        "lw t1, (t1)\n"
        // No fault, so set return to true
        "li %0, 1\n"
        ".p2align 2\n"
        // Always end up back here, restore the trap table
        "1:\n"
        "csrw mtvec, t0\n"
        // Now safe to restore interrupts
        "csrw mstatus, t2\n"
        : "=r" (result)
        :
        : "t0", "t1", "t2"
    );
    return result;
}
#endif

/*!
 * \brief Lookup a bootrom function by code. This method is forcibly inlined into the caller for FLASH/RAM sensitive code usage
 * \ingroup pico_bootrom
 * \param code the code
 * \return a pointer to the function, or NULL if the code does not match any bootrom function
 */
#pragma GCC diagnostic push
// diagnostic: GCC thinks near-zero value is a null pointer member access, but it's not
#pragma GCC diagnostic ignored "-Warray-bounds"
static __force_inline void *rom_func_lookup_inline(uint32_t code) {
#if PICO_RP2040
    rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn) rom_hword_as_ptr(BOOTROM_TABLE_LOOKUP_OFFSET);
    uint16_t *func_table = (uint16_t *) rom_hword_as_ptr(BOOTROM_FUNC_TABLE_OFFSET);
    return rom_table_lookup(func_table, code);
#else
#ifdef __riscv
    // todo amy remove this ROM size adjustment once 64k dev ROM is gone
    uint32_t rom_offset_adjust = rom_size_is_64k() ? 32 * 1024 : 0;
    // on RISC-V the code (a jmp) is actually embedded in the table
    rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn) (uintptr_t)*(uint16_t*)(BOOTROM_TABLE_LOOKUP_ENTRY_OFFSET + rom_offset_adjust);
    return rom_table_lookup(code, RT_FLAG_FUNC_RISCV);
#else
    // on ARM the function pointer is stored in the table, so we dereference it
    // via lookup() rather than lookup_entry()
    rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn) (uintptr_t)*(uint16_t*)(BOOTROM_TABLE_LOOKUP_OFFSET);
    if (pico_processor_state_is_nonsecure()) {
        return rom_table_lookup(code, RT_FLAG_FUNC_ARM_NONSEC);
    } else {
        return rom_table_lookup(code, RT_FLAG_FUNC_ARM_SEC);
    }
#endif
#endif
}
#pragma GCC diagnostic pop

/*!
 * \brief Reboot the device into BOOTSEL mode
 * \ingroup pico_bootrom
 *
 * This function reboots the device into the BOOTSEL mode ('usb boot").
 *
 * Facilities are provided to enable an "activity light" via GPIO attached LED for the USB Mass Storage Device,
 * and to limit the USB interfaces exposed.
 *
 * \param usb_activity_gpio_pin_mask 0 No pins are used as per a cold boot. Otherwise a single bit set indicating which
 *                               GPIO pin should be set to output and raised whenever there is mass storage activity
 *                               from the host.
 * \param disable_interface_mask value to control exposed interfaces
 *  - 0 To enable both interfaces (as per a cold boot)
 *  - 1 To disable the USB Mass Storage Interface
 *  - 2 To disable the USB PICOBOOT Interface
 */
#if PICO_RP2040
static inline void __attribute__((noreturn)) reset_usb_boot(uint32_t usb_activity_gpio_pin_mask,
                                                            uint32_t disable_interface_mask) {
    rom_reset_usb_boot_fn func = (rom_reset_usb_boot_fn) rom_func_lookup(ROM_FUNC_RESET_USB_BOOT);
    func(usb_activity_gpio_pin_mask, disable_interface_mask);
}
#else
void __attribute__((noreturn)) reset_usb_boot(uint32_t usb_activity_gpio_pin_mask, uint32_t disable_interface_mask);
#endif

#if !PICO_RP2040
static inline int boot_random(uint32_t out[4]) {
    rom_get_sys_info_fn func = (rom_get_sys_info_fn) rom_func_lookup(ROM_FUNC_GET_SYS_INFO);
    return func(out, 4, SYS_INFO_BOOT_RANDOM);
}

// todo SECURE only
static inline int rom_explicit_buy(uint8_t *buffer, uint32_t buffer_size) {
    rom_explicit_buy_fn func = (rom_explicit_buy_fn) rom_func_lookup(ROM_FUNC_EXPLICIT_BUY);
    return func(buffer, buffer_size);
}

#ifndef __riscv
static inline int rom_set_ns_api_permission(uint ns_api_num, bool allowed) {
    rom_set_ns_api_permission_fn func = (rom_set_ns_api_permission_fn) rom_func_lookup(ROM_FUNC_SET_NS_API_PERMISSION);
    return func(ns_api_num, allowed);
}
#endif

// todo SECURE only
static inline void* rom_validate_ns_buffer(const void *addr, uint32_t size, uint32_t write, uint32_t *ok) {
    rom_validate_ns_buffer_fn func = (rom_validate_ns_buffer_fn) rom_func_lookup(ROM_FUNC_VALIDATE_NS_BUFFER);
    return func(addr, size, write, ok);
}

static inline intptr_t rom_set_rom_callback(uint callback_num, bootrom_api_callback_generic_t funcptr) {
    rom_set_rom_callback_fn func = (rom_set_rom_callback_fn) rom_func_lookup(ROM_FUNC_SET_ROM_CALLBACK);
    return func(callback_num, funcptr);
}

#endif

#ifdef __cplusplus
}
#endif

#endif // !__ASSEMBLER__
#endif
