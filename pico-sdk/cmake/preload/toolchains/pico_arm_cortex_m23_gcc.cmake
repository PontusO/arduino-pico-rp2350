# todo there is probably a more "cmake" way of doing this going thru the standard path with our "PICO" platform
#  i.e. CMake<Lang>Information and whatnot
include(${CMAKE_CURRENT_LIST_DIR}/find_compiler.cmake)

set(CMAKE_SYSTEM_PROCESSOR cortex-m23)

if (NOT PICO_GCC_TRIPLE)
    if (DEFINED ENV{PICO_GCC_TRIPLE})
        set(PICO_GCC_TRIPLE $ENV{PICO_GCC_TRIPLE})
        message("PICO_GCC_TRIPLE set from environment: $ENV{PICO_GCC_TRIPLE}")
    else()
        set(PICO_GCC_TRIPLE arm-none-eabi)
    endif()
endif()

# todo amy should this be -mfloat-abi=hard?
set(PICO_COMMON_LANG_FLAGS " -mcpu=cortex-m23 -mthumb -march=armv8-m.base")
# todo probably need a setting here, also do we want `softfp`?
set(PICO_COMMON_LANG_FLAGS "${PICO_COMMON_LANG_FLAGS} -mfloat-abi=softfp")
if (NOT PICO_NO_CMSE)
    set(PICO_COMMON_LANG_FLAGS "${PICO_COMMON_LANG_FLAGS} -mcmse")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/pico_arm_gcc_common.cmake)