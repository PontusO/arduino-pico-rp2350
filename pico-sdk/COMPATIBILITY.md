## Differences between RP2350 and RP2040

Currently this is describing the hardware differences we will need to be aware of in the SDK. Over time this will turn into a user-facing file documenting the differences that are *not* abstracted by the SDK.

No code has been moved out of the `rp2_common` folder. There is a new set of register and struct headers in `src/rp2350`, and where minor differences exist, these are just handled with `#if PICO_RP2350` (if change is a feature specifically of RP2350) or `#if PICO_RP2040` (if change is due to a quirk of RP2040 that will consistently not be the case going forward). These could probably be extracted into hardware-specific layers in `src/rp2350` etc, but right now the aim is to keep the codebase compatible with both chips whilst keeping the non-additive parts of the diff small.

### Clocks
 
- Instead of a single tick signal from the watchdog, used globally by timer and SysTick etc, there is now a `TICKS` block which generates multiple independent tick signals. Each tick needs to be individually enabled during clocks initialisation.
- Clock divider registers in `clocks` have been split into separate integer/fractional registers.

### GPIO

- GPIO IRQ masking works differently (TODO haven't looked into exactly how)
- There will be more GPIOs
- There will be different packages with different numbers of GPIOs.
	- We will try to add an indirection layer in hardwdare so that, from software's perspective (and PIO's perspective etc) IO numbers from `0` to `io_count - 1` always correspond to consecutive pins anticlockwise round the package.
	- However, ADC pins can not be redirected in this way (digital only). So, ADC pins will appear on different "GPIO numbers" in different packages. They will always be the 4 highest-numbered GPIOs available in that package.

### XIP

- The cache flush works differently -- you need to explicitly iterate over cache lines. The `flush_flash_cache` bootrom symbol is still present and still works, you just need to be aware if you are flushing manually.
- There is no cache disable flag. XIP cache-as-SRAM works by pinning individual cache lines, not bulk-disabling the cache.
- The `XIP_NOCACHE` and `XIP_NOALLOC` address ranges are gone. References to these should be replaced with either `XIP` or `XIP_NOCACHE_NOALLOC`.
- Synopsys SSI will be replaced with new QSPI Memory Interface (QMI). Should be overall simpler to program, and much faster.
    - Bootrom flash programming routines will work the same as RP2040.
    - New second stages required
    - Out of reset, QMI will already be configured by default for a basic XIP mode (03h serial read).

### Timer

- There seem to be more than one timer now!... we need to look at priority (and IRQ placement)... also i have added a temporary TIMER_IRQ_0 define to hardware/timer.h for compatibility
- 
### Divider
- Not sure what the deal is but I see a DMB and the delay in the SIO divider; we need to figure out what this is and also check