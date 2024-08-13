if (NOT TARGET pico_multicore)
    pico_add_library(pico_multicore)
    target_include_directories(pico_multicore_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

    target_sources(pico_multicore INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/multicore.c)

    pico_mirrored_target_link_libraries(pico_multicore INTERFACE
            pico_sync
            hardware_irq)

    if (PICO_RISCV)
        pico_mirrored_target_link_libraries(pico_multicore INTERFACE hardware_riscv)
    endif()

    target_link_libraries(pico_multicore INTERFACE hardware_gpio_headers) # to determine if we should init GPIO copro (todo amy remove once we have common func)
endif()



