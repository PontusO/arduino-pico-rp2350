if (NOT (DEFINED PICO_COMPILER OR DEFINED CMAKE_TOOLCHAIN_FILE))
    pico_message("Defaulting PICO platform compiler to ${PICO_DEFAULT_COMPILER} since not specified.")
    set(PICO_COMPILER ${PICO_DEFAULT_COMPILER})
endif ()



