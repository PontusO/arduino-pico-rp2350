# todo there is probably a more "cmake" way of doing this going thru the standard path with our "PICO" platform
#  i.e. CMake<Lang>Information and whatnot
include(${CMAKE_CURRENT_LIST_DIR}/find_compiler.cmake)

set(CMAKE_SYSTEM_PROCESSOR hazard3)

if (DEFINED ENV{PICO_GCC_TRIPLE})
    set(PICO_GCC_TRIPLE $ENV{PICO_GCC_TRIPLE})
    message("PICO_GCC_TRIPLE set from environment: $ENV{PICO_GCC_TRIPLE}")
endif()
if (NOT PICO_GCC_TRIPLE)
    set(PICO_GCC_TRIPLE riscv32-corev-elf)
endif()

set(PICO_GCC_TRIPLE "${PICO_GCC_TRIPLE}" CACHE INTERNAL "")
set(PICO_COMMON_LANG_FLAGS " -march=rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp -mabi=ilp32")

include(${CMAKE_CURRENT_LIST_DIR}/pico_arm_gcc_common.cmake)

