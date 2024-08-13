# Open Questions
- Should we change `PICO_RP2350` to `__RP2350__` (perhaps as well)?
- platformery... RISCV is vs 2350, what about compiler? i.e. is arm_gcc always that - probably
# Bugs
- generic_03.s does not compile
- kitchen_sink does not either!

# Cmake 
# Structural Items
- pico_standard_link should move to rp2350/rp2040 directories.

  I would have done this already, however it has crt0.S stuff, new_delete.cpp etc. which should be moved and is 
  related to runtime_init

# This is a list of major todos

- DSB in divider (we need to figure out what this should be) 
- Need to add back non flash etc. builds; deferring this as we may well have linker script generation by then... in either case we need to fix up kitchen sink build again

# Minor todos

# M33 support

- new intrinsic - should we try to be CMSIS compatible?
- tt style macros
- hardware/divider optimized mul/add

## Forward port

Remember any changes to anything we have split must be forward ported

We have split
- boot stage 2
- pico_platform

## Pico & Amy
- generated linker scripts
  - jinja2 etc.. may cause windows problems...  
  - related to ^, should we try and remove dependency on native compiler (ship elf2uf2 and pioasm)
- default to unified syntax? (maybe backport)
- Remove int64_t from clock setup
- Add new delay_cycles method to platform.h
  - switch clock setup to use this
- Use distinct VTABLE on each core by default (on Amy)
- pico_standard_link split (but not move)
- Make runtime init more flexible
  
  This code be `#ifdef` based, but possibly plugin... basically we want to be able to support minimal CMSIS style, 
  different clock setups and of course in the Amy case the ability to be a client NS binary where the S binary has 
  done most of the setup
- ~~reordering of common/rp2_common in build~~

## Amy Only
- what is `dsb` at beginning of divider delay?

## Backport Sooner
- reordering of common/rp2_common in build. we have dependencies from common to more specific stuff

