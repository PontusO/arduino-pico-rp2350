# include everything needed to build against rp2040

set(RP2_VARIANT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR}/rp2040)
set(PICO_RP2040 "1" CACHE INTERNAL "")
set(PICO_RP2350 "0" CACHE INTERNAL "")
set(PICO_RISCV "0" CACHE INTERNAL "")
set(PICO_ARM "1" CACHE INTERNAL "")
set(PICO_CMSIS_DEVICE "RP2040" CACHE INTERNAL "")

include(rp2_common.cmake)

