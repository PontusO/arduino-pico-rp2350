# PICO_CMAKE_CONFIG: PICO_TOOLCHAIN_PATH, Path to search for compiler, default=none (i.e. search system paths), group=build
set(PICO_TOOLCHAIN_PATH "${PICO_TOOLCHAIN_PATH}" CACHE INTERNAL "")

# Set a default build type if none was specified
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Defaulting build type to '${default_build_type}' since not specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build, options are: 'Debug', 'Release', 'MinSizeRel', 'RelWithDebInfo'." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Default")
    error("Default build type is NOT supported")
endif()

# PICO_CMAKE_CONFIG: PICO_COMPILER, Optionally specifies a different compiler (other than pico_arm_gcc.cmake) - this is not yet fully supported, default=none, group=build
# If PICO_COMPILER is specified, set toolchain file to ${PICO_COMPILER}.cmake.
if (DEFINED PICO_COMPILER)
    # maintain backwards compatibility with RP2040 SDK compilers
    set(ORIG_PICO_COMPILER "${PICO_COMPILER}")
    if (PICO_COMPILER STREQUAL "pico_arm_gcc")
        if (PICO_PLATFORM STREQUAL "rp2040")
            set(PICO_COMPILER "pico_arm_cortex_m0plus_gcc")
        elseif(PICO_PLATFORM STREQUAL "rp2350")
            set(PICO_COMPILER "pico_arm_cortex_m33_gcc")
        endif()
    elseif(PICO_COMPILER STREQUAL "pico_arm_clang")
        if (PICO_PLATFORM STREQUAL "rp2040")
            set(PICO_COMPILER "pico_arm_cortex_m0plus_clang")
        elseif(PICO_PLATFORM STREQUAL "rp2350")
            set(PICO_COMPILER "pico_arm_cortex_m33_clang")
        endif()
    endif()
    if (NOT PICO_COMPILER STREQUAL ORIG_PICO_COMPILER)
        message("Accepting PICO_COMPILER value '${ORIG_PICO_COMPILER}' for compatibility, but using '${PICO_COMPILER}' instead")
    endif()
    if (DEFINED CMAKE_TOOLCHAIN_FILE)
        get_filename_component(toolchain "${CMAKE_TOOLCHAIN_FILE}" NAME_WE)
        if (NOT "${PICO_COMPILER}" STREQUAL "${toolchain}")
            message(WARNING "CMAKE_TOOLCHAIN_FILE is already defined to ${toolchain}.cmake, you\
                need to delete cache and reconfigure if you want to switch compiler.")
        endif ()
    else ()
        set(toolchain_dir "${CMAKE_CURRENT_LIST_DIR}/preload/toolchains")
        set(toolchain_file "${toolchain_dir}/${PICO_COMPILER}.cmake")
        if (EXISTS "${toolchain_file}")
            set(CMAKE_TOOLCHAIN_FILE "${toolchain_file}" CACHE INTERNAL "")
        else ()
            # todo improve message
            message(FATAL_ERROR "Toolchain file \"${PICO_COMPILER}.cmake\" does not exist, please\
                select one from \"cmake/toolchains\" folder.")
        endif ()
    endif ()
    message("PICO compiler is ${PICO_COMPILER}")
endif ()

unset(PICO_COMPILER CACHE)

