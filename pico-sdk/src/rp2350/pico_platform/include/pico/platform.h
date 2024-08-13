/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_PLATFORM_H
#define _PICO_PLATFORM_H

/** \file platform.h
 *  \defgroup pico_platform pico_platform
 *
 * Macros and definitions (and functions when included by non assembly code) for the RP2 family device / architecture
 * to provide a common abstraction over low level compiler / platform specifics.
 *
 * This header may be included by assembly code
 */

#include "hardware/platform_defs.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#ifdef __riscv
#include "hardware/regs/rvcsr.h"
#endif

// PICO_CONFIG: PICO_STACK_SIZE, Stack Size, min=0x100, default=0x800, advanced=true, group=pico_platform
#ifndef PICO_STACK_SIZE
#define PICO_STACK_SIZE _u(0x800)
#endif

// PICO_CONFIG: PICO_HEAP_SIZE, Heap size to reserve, min=0x100, default=0x800, advanced=true, group=pico_platform
#ifndef PICO_HEAP_SIZE
#define PICO_HEAP_SIZE _u(0x800)
#endif

// PICO_CONFIG: PICO_NO_RAM_VECTOR_TABLE, Enable/disable the RAM vector table, type=bool, default=0, advanced=true, group=pico_platform
#ifndef PICO_NO_RAM_VECTOR_TABLE
#define PICO_NO_RAM_VECTOR_TABLE 0
#endif

#ifndef PICO_RAM_VECTOR_TABLE_SIZE
#define PICO_RAM_VECTOR_TABLE_SIZE (VTABLE_FIRST_IRQ + NUM_IRQS)
#endif

// --- remainder of file is not included by assembly code ---

#ifndef __ASSEMBLER__

#if defined __GNUC__
#include <sys/cdefs.h>
// note LLVM defines __GNUC__
#ifdef __clang__
#define PICO_C_COMPILER_IS_CLANG 1
#else
#define PICO_C_COMPILER_IS_GNU 1
#endif
#elif defined __ICCARM__
#ifndef __aligned
#define __aligned(x)	__attribute__((__aligned__(x)))
#endif
#ifndef __always_inline
#define __always_inline __attribute__((__always_inline__))
#endif
#ifndef __noinline
#define __noinline      __attribute__((__noinline__))
#endif
#ifndef __packed
#define __packed        __attribute__((__packed__))
#endif
#ifndef __printflike
#define __printflike(a, b)
#endif
#ifndef __unused
#define __unused        __attribute__((__unused__))
#endif
#ifndef __used
#define __used          __attribute__((__used__))
#endif
#ifndef __CONCAT1
#define __CONCAT1(a, b) a ## b
#endif
#ifndef __CONCAT
#define __CONCAT(a, b)  __CONCAT1(a, b)
#endif
#ifndef __STRING
#define __STRING(a)     #a
#endif
/* Compatible definitions of GCC builtins */

static inline uint __builtin_ctz(uint x) {
  extern uint32_t __ctzsi2(uint32_t);
  return __ctzsi2(x);
}
#define __builtin_expect(x, y) (x)
#define __builtin_isnan(x) __iar_isnan(x)
#else
#error Unsupported toolchain
#endif

#include "pico/types.h"

// GCC_Like_Pragma(x) is a pragma on GNUC compatible compilers
#ifdef __GNUC__
#define GCC_Like_Pragma _Pragma
#else
#define GCC_Like_Pragma(x)
#endif

// Clang_Pragma(x) is a pragma on Clang only
#ifdef __clang__
#define Clang_Pragma _Pragma
#else
#define Clang_Pragma(x)
#endif

// GCC_Pragma(x) is a pragma on GCC only
#if PICO_C_COMPILER_IS_GNU
#define GCC_Pragma _Pragma
#else
#define GCC_Pragma(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Marker for an interrupt handler
 *  \ingroup pico_platform
 *
 * For example an IRQ handler function called my_interrupt_handler:
 *
 *     void __isr my_interrupt_handler(void) {
 */
#define __isr

/*! \brief Section attribute macro for placement in RAM after the `.data` section
 *  \ingroup pico_platform
 *
 * For example a 400 element `uint32_t` array placed after the .data section
 *
 *     uint32_t __after_data("my_group_name") a_big_array[400];
 *
 * The section attribute is `.after_data.<group>`
 *
 * \param group a string suffix to use in the section name to distinguish groups that can be linker
 *              garbage-collected independently
 */
#define __after_data(group) __attribute__((section(".after_data." group)))

/*! \brief Section attribute macro for placement not in flash (i.e in RAM)
 *  \ingroup pico_platform
 *
 * For example a 3 element `uint32_t` array placed in RAM (even though it is `static const`)
 *
 *     static const uint32_t __not_in_flash("my_group_name") an_array[3];
 *
 * The section attribute is `.time_critical.<group>`
 *
 * \param group a string suffix to use in the section name to distinguish groups that can be linker
 *              garbage-collected independently
 */
#define __not_in_flash(group) __attribute__((section(".time_critical." group)))

/*! \brief Section attribute macro for placement in the SRAM bank 4 (known as "scratch X")
 *  \ingroup pico_platform
 *
 * Scratch X is commonly used for critical data and functions accessed only by one core (when only
 * one core is accessing the RAM bank, there is no opportunity for stalls)
 *
 * For example a `uint32_t` variable placed in "scratch X"
 *
 *     uint32_t __scratch_x("my_group_name") foo = 23;
 *
 * The section attribute is `.scratch_x.<group>`
 *
 * \param group a string suffix to use in the section name to distinguish groups that can be linker
 *              garbage-collected independently
 */
#define __scratch_x(group) __attribute__((section(".scratch_x." group)))

/*! \brief Section attribute macro for placement in the SRAM bank 5 (known as "scratch Y")
 *  \ingroup pico_platform
 *
 * Scratch Y is commonly used for critical data and functions accessed only by one core (when only
 * one core is accessing the RAM bank, there is no opportunity for stalls)
 *
 * For example a `uint32_t` variable placed in "scratch Y"
 *
 *     uint32_t __scratch_y("my_group_name") foo = 23;
 *
 * The section attribute is `.scratch_y.<group>`
 *
 * \param group a string suffix to use in the section name to distinguish groups that can be linker
 *              garbage-collected independently
 */
#define __scratch_y(group) __attribute__((section(".scratch_y." group)))

/*! \brief Section attribute macro for data that is to be left uninitialized
 *  \ingroup pico_platform
 *
 * Data marked this way will retain its value across a reset (normally uninitialized data - in the .bss
 * section) is initialized to zero during runtime initialization
 *
 * For example a `uint32_t` foo that will retain its value if the program is restarted by reset.
 *
 *     uint32_t __uninitialized_ram(foo);
 *
 * The section attribute is `.uninitialized_data.<group>`
 *
 * \param group a string suffix to use in the section name to distinguish groups that can be linker
 *              garbage-collected independently
 */
#define __uninitialized_ram(group) __attribute__((section(".uninitialized_data." #group))) group

/*! \brief Section attribute macro for placement in flash even in a COPY_TO_RAM binary
 *  \ingroup pico_platform
 *
 * For example a `uint32_t` variable explicitly placed in flash (it will hard fault if you attempt to write it!)
 *
 *     uint32_t __in_flash("my_group_name") foo = 23;
 *
 * The section attribute is `.flashdata.<group>`
 *
 * \param group a string suffix to use in the section name to distinguish groups that can be linker
 *              garbage-collected independently
 */
#define __in_flash(group) __attribute__((section(".flashdata." group)))

/*! \brief Indicates a function should not be stored in flash
 *  \ingroup pico_platform
 *
 * Decorates a function name, such that the function will execute from RAM (assuming it is not inlined
 * into a flash function by the compiler)
 *
 * For example a function called my_func taking an int parameter:
 *
 *     void __not_in_flash_func(my_func)(int some_arg) {
 *
 * The function is placed in the `.time_critical.<func_name>` linker section
 *
 * \see __no_inline_not_in_flash_func
 */
#define __not_in_flash_func(func_name) __not_in_flash(__STRING(func_name)) func_name

/*! \brief Indicates a function is time/latency critical and should not run from flash
 *  \ingroup pico_platform
 *
 * Decorates a function name, such that the function will execute from RAM (assuming it is not inlined
 * into a flash function by the compiler) to avoid possible flash latency. Currently this macro is identical
 * in implementation to `__not_in_flash_func`, however the semantics are distinct and a `__time_critical_func`
 * may in the future be treated more specially to reduce the overhead when calling such function from a flash
 * function.
 *
 * For example a function called my_func taking an int parameter:
 *
 *     void __time_critical(my_func)(int some_arg) {
 *
 * The function is placed in the `.time_critical.<func_name>` linker section
 *
 * \see __not_in_flash_func
 */
#define __time_critical_func(func_name) __not_in_flash_func(func_name)

/*! \brief Indicate a function should not be stored in flash and should not be inlined
 *  \ingroup pico_platform
 *
 * Decorates a function name, such that the function will execute from RAM, explicitly marking it as
 * noinline to prevent it being inlined into a flash function by the compiler
 *
 * For example a function called my_func taking an int parameter:
 *
 *     void __no_inline_not_in_flash_func(my_func)(int some_arg) {
 *
 * The function is placed in the `.time_critical.<func_name>` linker section
 */
#define __no_inline_not_in_flash_func(func_name) __noinline __not_in_flash_func(func_name)

#define __packed_aligned __packed __aligned(4)

/*! \brief Attribute to force inlining of a function regardless of optimization level
 *  \ingroup pico_platform
 *
 *  For example my_function here will always be inlined:
 *
 *      int __force_inline my_function(int x) {
 *
 */

#if PICO_C_COMPILER_IS_GNU && (__GNUC__ <= 6 || (__GNUC__ == 7 && (__GNUC_MINOR__ < 3 || !defined(__cplusplus))))
#define __force_inline inline __always_inline
#else
#define __force_inline __always_inline
#endif

/*! \brief Macro to determine the number of elements in an array
 *  \ingroup pico_platform
 */
#ifndef count_of
#define count_of(a) (sizeof(a)/sizeof((a)[0]))
#endif

/*! \brief Macro to return the maximum of two comparable values
 *  \ingroup pico_platform
 */
#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#endif

/*! \brief Macro to return the minimum of two comparable values
 *  \ingroup pico_platform
 */
#ifndef MIN
#define MIN(a, b) ((b)>(a)?(a):(b))
#endif

#ifndef __riscv
#define pico_default_asm(...) __asm (".syntax unified\n" __VA_ARGS__)
#define pico_default_asm_volatile(...) __asm volatile (".syntax unified\n" __VA_ARGS__)
#define pico_default_asm_goto(...) __asm goto (".syntax unified\n" __VA_ARGS__)
#else
#define pico_default_asm(...) __asm (__VA_ARGS__)
#define pico_default_asm_volatile(...) __asm volatile (__VA_ARGS__)
#define pico_default_asm_goto(...) __asm goto (__VA_ARGS__)
#endif

/*! \brief Execute a breakpoint instruction
 *  \ingroup pico_platform
 */
static __force_inline void __breakpoint(void) {
#ifdef __riscv
    __asm ("ebreak");
#else
    pico_default_asm_volatile ("bkpt #0" : : : "memory");
#endif
}

/*! \brief Ensure that the compiler does not move memory access across this method call
 *  \ingroup pico_platform
 *
 *  For example in the following code:
 *
 *      *some_memory_location = var_a;
 *      __compiler_memory_barrier();
 *      uint32_t var_b = *some_other_memory_location
 *
 * The compiler will not move the load from `some_other_memory_location` above the memory barrier (which it otherwise
 * might - even above the memory store!)
 */
__force_inline static void __compiler_memory_barrier(void) {
    pico_default_asm_volatile ("" : : : "memory");
}

/*! \brief Macro for converting memory addresses to 32 bit addresses suitable for DMA
 *  \ingroup pico_platform
 *
 *  This is just a cast to `uintptr_t` on the RP2040, however you may want to use this when developing code
 *  that also runs in "host" mode. If the host mode is 64 bit and you are embedding data pointers
 *  in other data (e.g. DMA chaining), then there is a need in "host" mode to convert a 64 bit native
 *  pointer to a 32 bit value for storage, which can be done using this macro.
 */
#define host_safe_hw_ptr(x) ((uintptr_t)(x))
#define native_safe_hw_ptr(x) host_safe_hw_ptr(x)


/*! \brief Panics with the message "Unsupported"
 *  \ingroup pico_platform
 *  \see panic
 */
void __attribute__((noreturn)) panic_unsupported(void);

/*! \brief Displays a panic message and halts execution
 *  \ingroup pico_platform
 *
 * An attempt is made to output the message to all registered STDOUT drivers
 * after which this method executes a BKPT instruction.
 *
 * @param fmt format string (printf-like)
 * @param ...  printf-like arguments
 */
void __attribute__((noreturn)) panic(const char *fmt, ...);

#ifdef NDEBUG
#define panic_compact(...) panic(__VA_ARGS__)
#else
#define panic_compact(...) panic("")
#endif

// PICO_CONFIG: PICO_NO_FPGA_CHECK, Remove the FPGA platform check for small code size reduction, type=bool, default=0, advanced=true, group=pico_runtime
#ifndef PICO_NO_FPGA_CHECK
#define PICO_NO_FPGA_CHECK 0
#endif

#if PICO_NO_FPGA_CHECK
static inline bool running_on_fpga(void) {return false;}
#else
bool running_on_fpga(void);
#endif
// todo amy remove me
int running_in_sim(void);

/*! \brief Returns the RP2040 chip revision number
 *  \ingroup pico_platform
 * @return the RP2040 chip revision number (1 for B0/B1, 2 for B2)
 */
uint8_t rp2040_chip_version(void);

/*! \brief Returns the RP2040 rom version number
 *  \ingroup pico_platform
 * @return the RP2040 rom version number (1 for RP2040-B0, 2 for RP2040-B1, 3 for RP2040-B2)
 */
static inline uint8_t rp2040_rom_version(void) {
GCC_Pragma("GCC diagnostic push")
GCC_Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
    return *(uint8_t*)0x13;
GCC_Pragma("GCC diagnostic pop")
}

/*! \brief No-op function for the body of tight loops
 *  \ingroup pico_platform
 *
 * No-op function intended to be called by any tight hardware polling loop. Using this ubiquitously
 * makes it much easier to find tight loops, but also in the future \#ifdef-ed support for lockup
 * debugging might be added
 */
static __force_inline void tight_loop_contents(void) {}

/*! \brief Multiply two integers using an assembly `MUL` instruction
 *  \ingroup pico_platform
 *
 * This multiplies a by b using multiply instruction using the ARM mul instruction regardless of values (the compiler
 * might otherwise choose to perform shifts/adds), i.e. this is a 1 cycle operation.
 *
 * \param a the first operand
 * \param b the second operand
 * \return a * b
 */
__force_inline static int32_t __mul_instruction(int32_t a, int32_t b) {
#ifdef __riscv
    __asm ("mul %0, %0, %1" : "+l" (a) : "l" (b) : );
#else
    pico_default_asm ("muls %0, %1"     : "+l" (a) : "l" (b) : "cc");
#endif
    return a;
}

/*! \brief multiply two integer values using the fastest method possible
 *  \ingroup pico_platform
 *
 * Efficiently multiplies value a by possibly constant value b.
 *
 * If b is known to be constant and not zero or a power of 2, then a mul instruction is used rather than gcc's default
 * which is often a slow combination of shifts and adds. If b is a power of 2 then a single shift is of course preferable
 * and will be used
 *
 * \param a the first operand
 * \param b the second operand
 * \return a * b
 */
#define __fast_mul(a, b) __builtin_choose_expr(__builtin_constant_p(b) && !__builtin_constant_p(a), \
(__builtin_popcount(b) >= 2 ? __mul_instruction(a,b) : (a)*(b)), \
(a)*(b))

/*! \brief Utility macro to assert two types are equivalent.
 *  \ingroup pico_platform
 *
 *  This macro can be useful in other macros along with `typeof` to assert that two parameters are of equivalent type
 *  (or that a single parameter is of an expected type)
 */
#define __check_type_compatible(type_a, type_b) static_assert(__builtin_types_compatible_p(type_a, type_b), __STRING(type_a) " is not compatible with " __STRING(type_b));

#define needs_love_rp2350() ({_Pragma ("GCC warning \"rp2350 code missing\""); panic_unsupported(); })
#define needs_love_rp2350_skip() ((void)0)
#define needs_love_riscv() ({_Pragma ("GCC warning \"RISC-V code missing\""); panic_unsupported(); })
#define needs_love_riscv_skip() ((void)0)

/*! \brief Get the current exception level on this core
 *  \ingroup pico_platform
 *
 * On Cortex-M this is the exception number defined in the architecture
 * reference, which is equal to VTABLE_FIRST_IRQ + irq num if inside an
 * interrupt handler. (VTABLE_FIRST_IRQ is defined in platform_defs.h).
 *
 * On Hazard3, this function returns VTABLE_FIRST_IRQ + irq num if inside of
 * an external IRQ handler (or a fault from such a handler), and 0 otherwise,
 * generally aligning with the Cortex-M values.
 *
 * \return the exception number if the CPU is handling an exception, or 0 otherwise
 */
static __force_inline uint __get_current_exception(void) {
#ifdef __riscv
    uint32_t meicontext;
    pico_default_asm_volatile (
        "csrr %0, %1\n"
        : "=r" (meicontext) : "i" (RVCSR_MEICONTEXT_OFFSET)
    );
    if (meicontext & RVCSR_MEICONTEXT_NOIRQ_BITS) {
        return 0;
    } else {
        return VTABLE_FIRST_IRQ + (
            (meicontext & RVCSR_MEICONTEXT_IRQ_BITS) >> RVCSR_MEICONTEXT_IRQ_LSB
        );
    }
#else
    uint exception;
    pico_default_asm_volatile (
        "mrs %0, ipsr\n"
        "uxtb %0, %0\n"
        : "=l" (exception)
    );
    return exception;
#endif
}

#define WRAPPER_FUNC(x) __wrap_ ## x
#define REAL_FUNC(x) __real_ ## x

/*! \brief Helper method to busy-wait for at least the given number of cycles
 *  \ingroup pico_platform
 *
 * This method is useful for introducing very short delays.
 *
 * This method busy-waits in a tight loop for the given number of system clock cycles. The total wait time is only accurate to within 2 cycles,
 * and this method uses a loop counter rather than a hardware timer, so the method will always take longer than expected if an
 * interrupt is handled on the calling core during the busy-wait; you can of course disable interrupts to prevent this.
 *
 * You can use \ref clock_get_hz(clk_sys) to determine the number of clock cycles per second if you want to convert an actual
 * time duration to a number of cycles.
 *
 * \param minimum_cycles the minimum number of system clock cycles to delay for
 */
static inline void busy_wait_at_least_cycles(uint32_t minimum_cycles) {
    pico_default_asm_volatile (
#ifdef __riscv
    // Note the range is halved on RISC-V due to signed comparison (no carry flag)
    ".option push\n"
    ".option norvc\n" // force 32 bit addi, so branch prediction guaranteed
    ".p2align 2\n"
    "1: \n"
    "addi %0, %0, -2 \n"
    "bgez %0, 1b\n"
    ".option pop"
#else
    "1: subs %0, #3\n"
    "bcs 1b\n"
#endif
    : "+r" (minimum_cycles) : : "cc", "memory"
    );
}

/*! \brief Get the current core number
 *  \ingroup pico_platform
 *
 * \return The core number the call was made from
 */
__force_inline static uint get_core_num(void) {
    return (*(uint32_t *) (SIO_BASE + SIO_CPUID_OFFSET));
}

/*! \brief Return true if executing in the NonSecure state (Arm-only)
 *  \ingroup pico_platform
 *
 * \return True if currently executing in the NonSecure state on an Arm processor
 */
__force_inline static bool pico_processor_state_is_nonsecure(void) {
#ifndef __riscv
    // IDAU-Exempt addresses return S=1 when tested in the Secure state,
    // whereas executing a tt in the NonSecure state will always return S=0.
    uint32_t tt;
    pico_default_asm_volatile (
        "movs %0, #0\n"
        "tt %0, %0\n"
        : "=r" (tt) : : "cc"
    );
    return !(tt & (1u << 22));
#endif
    // NonSecure is an Arm concept, there is nothing meaningful to return
    // here. Note it's not possible in general to detect whether you are
    // executing in U-mode as, for example, M-mode is classically
    // virtualisable in U-mode.
    return false;
}

#ifdef __cplusplus
}
#endif

#else // __ASSEMBLER__

#if defined __GNUC__
// note LLVM defines __GNUC__
#ifdef __clang__
#define PICO_ASSEMBLER_IS_CLANG 1
#else
#define PICO_ASSEMBLER_IS_GNU 1
#endif
#elif defined __ICCARM__
#else
#error Unsupported toolchain
#endif

#define WRAPPER_FUNC_NAME(x) __wrap_##x
#define SECTION_NAME(x) .text.##x
#define RAM_SECTION_NAME(x) .time_critical.##x

#endif // !__ASSEMBLER__

#endif
