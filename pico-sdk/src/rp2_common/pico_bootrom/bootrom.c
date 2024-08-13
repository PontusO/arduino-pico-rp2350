/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/bootrom.h"

/// \tag::table_lookup[]

void *rom_func_lookup(uint32_t code) {
    return rom_func_lookup_inline(code);
}

#pragma GCC diagnostic push
// diagnostic: GCC thinks near-zero value is a null pointer member access, but it's not
#pragma GCC diagnostic ignored "-Warray-bounds"
void *rom_data_lookup(uint32_t code) {
#if PICO_RP2040
    rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn) rom_hword_as_ptr(BOOTROM_TABLE_LOOKUP_OFFSET);
    uint16_t *data_table = (uint16_t *) rom_hword_as_ptr(BOOTROM_DATA_TABLE_OFFSET);
    return rom_table_lookup(data_table, code);
#else
    rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn) (uintptr_t)*(uint16_t*)(BOOTROM_TABLE_LOOKUP_OFFSET);
    return rom_table_lookup(code, RT_FLAG_DATA);
#endif
}
#pragma GCC diagnostic pop
/// \end::table_lookup[]

bool rom_funcs_lookup(uint32_t *table, unsigned int count) {
    bool ok = true;
    for (unsigned int i = 0; i < count; i++) {
        table[i] = (uintptr_t) rom_func_lookup(table[i]);
        if (!table[i]) ok = false;
    }
    return ok;
}

#if !PICO_RP2040
#include "boot/picoboot.h"
// todo
void __attribute__((noreturn)) reset_usb_boot(uint32_t usb_activity_gpio_pin_mask, uint32_t disable_interface_mask) {
    rom_reboot_fn func = (rom_reboot_fn) rom_func_lookup(ROM_FUNC_REBOOT);
    uint32_t flags = disable_interface_mask;
    needs_love_rp2350_skip(); // do we want to improve the interface to this ^now, or just make a new API
    if (usb_activity_gpio_pin_mask) {
        flags |= BOOTSEL_FLAG_GPIO_PIN_SPECIFIED;
        // the parameter is actually the pin number, but we only care if BOOTSEL_FLAG_GPIO_PIN_SPECIFIED
        usb_activity_gpio_pin_mask = __builtin_ctz(usb_activity_gpio_pin_mask);
    }
    func(REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL | REBOOT2_FLAG_NO_RETURN_ON_SUCCESS, 10, flags, usb_activity_gpio_pin_mask);
    __builtin_unreachable();
}
#endif
