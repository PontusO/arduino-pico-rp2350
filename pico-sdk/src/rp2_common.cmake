# include targets for all for PICO on device

enable_language(ASM)

function(pico_get_runtime_output_directory TARGET output_path_name)
    get_target_property(${TARGET}_runtime_directory ${TARGET} RUNTIME_OUTPUT_DIRECTORY)
    if (${TARGET}_runtime_directory)
        get_filename_component(output_path "${${TARGET}_runtime_directory}"
                REALPATH BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
        file(MAKE_DIRECTORY "${output_path}")
        set(output_path "${output_path}/")
    else()
        set(output_path "")
    endif()
    set(${output_path_name} ${output_path} PARENT_SCOPE)
endfunction()

function(pico_add_hex_output TARGET)
    pico_get_runtime_output_directory(${TARGET} output_path)
    add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${TARGET}> ${output_path}$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.hex VERBATIM)
endfunction()

function(pico_add_bin_output TARGET)
    pico_get_runtime_output_directory(${TARGET} output_path)
    add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${TARGET}> ${output_path}$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.bin VERBATIM)
endfunction()

function(pico_add_dis_output TARGET)
    pico_get_runtime_output_directory(${TARGET} output_path)
    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_OBJDUMP} -h $<TARGET_FILE:${TARGET}> > ${output_path}$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.dis
            COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${TARGET}> >> ${output_path}$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.dis
            VERBATIM
            )
endfunction()

function(pico_add_extra_outputs TARGET)
    pico_add_hex_output(${TARGET})
    pico_add_bin_output(${TARGET})
    pico_add_dis_output(${TARGET})
    pico_add_map_output(${TARGET})

    # PICO_CMAKE_CONFIG: PICO_NO_TARGET_NAME, Don't define PICO_TARGET_NAME, type=bool, default=0, group=build
    # PICO_BUILD_DEFINE: PICO_TARGET_NAME, The name of the build target being compiled (unless PICO_NO_TARGET_NAME set in build), type=string, default=target name, group=build
    if (NOT PICO_NO_TARGET_NAME)
        target_compile_definitions(${TARGET} PRIVATE
                PICO_TARGET_NAME="${TARGET}"
                )
    endif()

    if (PICO_SYMLINK_ELF_AS_FILENAME)
        add_custom_target(${TARGET}_symlinked)
        add_dependencies(${TARGET}_symlinked ${TARGET})

        add_custom_command(TARGET ${TARGET}_symlinked POST_BUILD
                COMMAND rm -f "${PICO_SYMLINK_ELF_AS_FILENAME}"
                COMMAND ln -s -r $<TARGET_FILE:${TARGET}> "${PICO_SYMLINK_ELF_AS_FILENAME}"
                COMMENT "Symlinking from ${PICO_SYMLINK_ELF_AS_FILENAME} to ${TARGET}"
                )
    endif ()
    # PICO_CMAKE_CONFIG: PICO_NO_UF2, Disable UF2 output, type=bool, default=0, group=build
    if (NOT PICO_NO_UF2)
        pico_add_uf2_output(${TARGET})
    endif()
endfunction()

add_subdirectory(common)
add_subdirectory(${RP2_VARIANT_LIST_DIR})
add_subdirectory(rp2_common)

# PICO_CMAKE_CONFIG: PICO_NO_HARDWARE, OPTION: Whether the build is not targeting an RP2040 or RP2350 device, type=bool, default=1 when PICO_PLATFORM is host, 0 otherwise, group=build
# PICO_BUILD_DEFINE: PICO_NO_HARDWARE, Whether the build is not targeting an RP2040 or RP2350 device, type=bool, default=1 when PICO_PLATFORM is host, 0 otherwise, group=build
set(PICO_NO_HARDWARE "0" CACHE INTERNAL "")
# PICO_CMAKE_CONFIG: PICO_ON_DEVICE, OPTION: Whether the build is targeting an RP2040 or RP2350 device, type=bool, default=0 when PICO_PLATFORM is host, 1 otherwise, group=build
# PICO_BUILD_DEFINE: PICO_ON_DEVICE, Whether the build is targeting an RP2040 or RP2350 device, type=bool, default=0 when PICO_PLATFORM is host, 1 otherwise, group=build
set(PICO_ON_DEVICE "1" CACHE INTERNAL "")

set(CMAKE_EXECUTABLE_SUFFIX .elf PARENT_SCOPE)
