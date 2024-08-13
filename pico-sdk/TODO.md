Complain if package define not matching PICO_PLATFORM
Compile time
Default board should be dependent on PICO_PLATFORM,
Can we default the platform based on board directory ;-) grep board header
* memcpy from GCC does not work for unaligned copies in peripheral space
* Make a header that includes m33.h or m0plus.h to avoid #ifdef in each inclusion; however we should see if there is
  any abstraction we can do, otherwise there isn't a whole lot of point
* ~~remove HACK build types~~
* ~~move hardware_div into rp2040/rp2350~~ no; multiple trees is hard due to `if (TARGET)`
* ~~move harrdware_rtc into rp2040/rp2350~~ no
* picotool and setting values/binary_info
* ~~PICO_HACK_ binary types need to be descoped into something that does just enough to let us build the bootrom, and then renamed~~

### reorg of pico_standard_link
* Fix up assertion in memmap.ld (note memmaps should not be shared anyway)
* make it easier to configure stacks (and switch them)

### random
* replace cycle loops (e.g. clocks.c) with busy_wait_at_least_cycles
* option (and default to?) separate IRQ tables on each core
* maybe point to binary_info from IMAGE_DEF? (perhaps not)- done?
* need to add generated cmsis RP2350.h and m33 headers - currently cmsis_test commented out
* 
### Hardware

- ticks_hw_t struct has magic 5 (either static_assert or use the #define)

#### GPIO
* use new coprocessor on ARM
* 
### RP2040

* Why hasn't RP2040 gained a hardware/structs/nvic.h - no need for code to be different.

#### Clang

* Check bitops - there is no __ctzll for example

#### floating point

* check out of range sin/cos etc - saw a crash somwhere
* need to init on both cores
* missing l2d, fixed2float etc.
* bug in GCC < 11 something ... empty source files barf if mixed with softfp(maybe just if first one?) - seen in khan

MO:

* review how sqrtf is linked (and which version - looks like GCC with errno handling) - appears to be in RAM
   - wrapping of sqrtf is different from that of sqrt
* ui64 t; double u; u=t/1e6; gives undefined reference to __wrap___aeabi_ul2d
   need a list of all these that are missing
* introduced PICO_FLOAT_IN_RAM parallel to PICO_DOUBLE_IN_RAM (which doesn't seem to be used anyway)
* we have __aeabi_dcmpeq etc, do we need __aeabi_fcmpeq etc?
* multiply optimisation in platform.h no longer correct as we have ADD R1,R0,R0,LSL#3, RSB R1,R0,R0,LSL#4 etc in a single cycle
* update copyright years

### RISC-V

* Shared IRQs
* Boot2
* Support duplication of RAM vector table once this is added for Arm
* Add `PICO_PLATFORM=rp2350-arm` as an alternative spelling of `rp2350`

### Security

* what do we need to set msplim to (can we know in advance)

### Build

* note that the `try_compile` stuff in CMake from SDK1.5.0 breaks PICO_GCC_TRIPLE (basically we can't pass variables 
  to the toolchain)... we should remove that
  * Note I think this change makes sense; we should pick explicit toolchain files based on CMake vars in the main build.

### Partition table

* we want this to be supported on Mu - can we forward to a block off end of binary... 4K

### TinyUSB

* make sure we didn't bloat RP2040 with RP2350 code (i.e. make sure it is #ifdef-ed)
