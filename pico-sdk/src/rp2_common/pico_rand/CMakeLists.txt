pico_add_library(pico_rand)
pico_add_library(pico_rand_default)
pico_add_library(pico_rand_software)

target_include_directories(pico_rand_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if (0 AND PICO_RP2350) # going to be merged back in as antoher input to the existing rand
    pico_add_library(pico_rand_hardware)
    target_sources(pico_rand_hardware INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/rand_hardware.c
            )
    pico_mirrored_target_link_libraries(pico_rand_hardware INTERFACE hardware_structs hardware_timer)
    target_link_libraries(pico_rand_default INTERFACE pico_rand_hardware)
else()
    target_link_libraries(pico_rand_default INTERFACE pico_rand_software)
endif()

set(PICO_DEFAULT_RAND_IMPL pico_rand_default)

target_link_libraries(pico_rand INTERFACE
        $<IF:$<BOOL:$<TARGET_PROPERTY:PICO_TARGET_RAND_IMPL>>,$<TARGET_PROPERTY:PICO_TARGET_RAND_IMPL>,${PICO_DEFAULT_RAND_IMPL}>)

target_sources(pico_rand_software INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/rand_software.c
)

pico_mirrored_target_link_libraries(pico_rand_software INTERFACE
        pico_unique_id
        hardware_clocks
        hardware_timer
        hardware_sync)

macro(pico_set_rand_implementation TARGET IMPL)
    get_target_property(target_type ${TARGET} TYPE)
    if ("EXECUTABLE" STREQUAL "${target_type}")
        set_target_properties(${TARGET} PROPERTIES PICO_TARGET_RAND_IMPL "pico_rand_${IMPL}")
    else()
        message(FATAL_ERROR "rand implementation must be set on executable not library")
    endif()
endmacro()
