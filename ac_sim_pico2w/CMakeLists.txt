# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(ac_sim_pico2w C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ac_sim_pico2w)

target_sources(ac_sim_pico2w PRIVATE 
    ac_sim_pico2w.cpp 
    app_math.cpp
    scope_out.cpp
)

pico_set_program_name(ac_sim_pico2w "ac_sim_pico2w")
pico_set_program_version(ac_sim_pico2w "0.4")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ac_sim_pico2w 1)
pico_enable_stdio_usb(ac_sim_pico2w 0)

# Add the standard library to the build
target_link_libraries(ac_sim_pico2w
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(ac_sim_pico2w PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# for math defines such as M_PI
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)

# Add any user requested libraries
target_link_libraries(ac_sim_pico2w 
        hardware_pwm
        )

pico_add_extra_outputs(ac_sim_pico2w)

