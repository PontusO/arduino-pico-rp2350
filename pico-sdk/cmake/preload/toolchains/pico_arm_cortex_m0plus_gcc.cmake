# todo there is probably a more "cmake" way of doing this going thru the standard path with our "PICO" platform
#  i.e. CMake<Lang>Information and whatnot
include(${CMAKE_CURRENT_LIST_DIR}/find_compiler.cmake)

set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus)

if (NOT PICO_GCC_TRIPLE)
    if (DEFINED ENV{PICO_GCC_TRIPLE})
        set(PICO_GCC_TRIPLE $ENV{PICO_GCC_TRIPLE})
        message("PICO_GCC_TRIPLE set from environment: $ENV{PICO_GCC_TRIPLE}")
    else()
        set(PICO_GCC_TRIPLE arm-none-eabi)
        #pico_message_debug("PICO_GCC_TRIPLE defaulted to arm-none-eabi")
    endif()
endif()

# on ARM -mcpu should not be mixed with -march
set(PICO_COMMON_LANG_FLAGS " -mcpu=cortex-m0plus -mthumb")

include(${CMAKE_CURRENT_LIST_DIR}/pico_arm_gcc_common.cmake)