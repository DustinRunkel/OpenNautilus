cmake_minimum_required(VERSION 3.20...3.30)
project(OpenNautilus
    VERSION 1.0.0
    LANGUAGES C)

set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/)
set(PICO_PLATFORM rp2040)
set(PICO_SDK_FETCH_FROM_GIT ON)

add_subdirectory(config)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/pico_sdk_import.cmake)
