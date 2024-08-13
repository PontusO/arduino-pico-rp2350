# todo there is probably a more "cmake" way of doing this going thru the standard path with our "PICO" platform
#  i.e. CMake<Lang>Information and whatnot
include(${CMAKE_CURRENT_LIST_DIR}/find_compiler.cmake)

set(CMAKE_SYSTEM_PROCESSOR hazard3)

if (NOT PICO_GCC_TRIPLE)
    if (DEFINED ENV{PICO_GCC_TRIPLE})
        set(PICO_GCC_TRIPLE $ENV{PICO_GCC_TRIPLE})
        message("PICO_GCC_TRIPLE set from environment: $ENV{PICO_GCC_TRIPLE}")
    else()
        set(PICO_GCC_TRIPLE riscv32-unknown-elf)
    endif()
endif()

set(PICO_COMMON_LANG_FLAGS " -march=rv32imac_zicsr_zifencei_zba_zbb_zbc_zbs_zbkb -mabi=ilp32")

include(${CMAKE_CURRENT_LIST_DIR}/pico_arm_gcc_common.cmake)

