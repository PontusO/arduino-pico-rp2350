# include everything needed to build against rp2350-riscv

set(PICO_RP2040 "0" CACHE INTERNAL "")
set(PICO_RP2350 "1" CACHE INTERNAL "")
set(PICO_RISCV "1" CACHE INTERNAL "")
set(PICO_ARM "0" CACHE INTERNAL "")
set(RP2_VARIANT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR}/rp2350)
set(PICO_PIO_VERSION "1" CACHE INTERNAL "")
include(rp2_common.cmake)

