cmake_minimum_required(VERSION 3.20...3.30)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

set(PICO_SDK_FETCH_FROM_GIT ON)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/pico_sdk_import.cmake)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

set(PICO_PLATFORM rp2040)
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT GCC_RP2040)


project(OpenNautilus
    VERSION 1.0.0
    LANGUAGES C CXX ASM)

pico_sdk_init()

add_subdirectory(config)

add_subdirectory(FreeRTOS-Kernel)


add_executable(${PROJECT_NAME}
    main.c
)

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib 
    freertos_kernel
    freertos_config
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

target_compile_options( ${PROJECT_NAME} PUBLIC
        ### Gnu/Clang C Options
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>
        )