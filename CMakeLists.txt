cmake_minimum_required(VERSION 3.12)

# Set the toolchain file
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Set the project name and version
project(STM32F407_Project C ASM)
set(CMAKE_PROJECT_VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add compile options
add_compile_options(-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# Add necessary definitions
add_definitions(-DUSE_HAL_DRIVER -DSTM32F407xx)

# Linker options
add_link_options(-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
add_link_options(-Wl,-gc-sections,--print-memory-usage)
add_link_options(-specs=nano.specs -specs=nosys.specs)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

# Source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/*.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
)

# Create the executable
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${CMAKE_SOURCE_DIR}/startup_stm32f407vgtx.s)

# Link the libraries
target_link_options(${PROJECT_NAME}.elf PRIVATE -T${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld)

# Print executable size
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
)

# Create hex file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
)

# Create bin file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
)
