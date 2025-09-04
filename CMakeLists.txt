# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和支持的语言
project(Project C CXX ASM)

#切换编译环境
set(CMAKE_SYSTEM_NAME          Generic) 
set(CMAKE_SYSTEM_PROCESSOR     arm)
set(CMAKE_C_COMPILER           arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER         arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER         arm-none-eabi-gcc)
set(CMAKE_OBJCOPY              arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP              arm-none-eabi-objdump)

# 设置编译选项
add_compile_options(
    -mcpu=cortex-m3
    -mthumb
    -mthumb-interwork
    -ffunction-sections
    -fdata-sections
    -fno-common
    -Wall
    -Wextra
    -O
    -g
)

# 设置链接选项
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/driver/stm32_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS
    "-mcpu=cortex-m3 -mthumb \
     -T${LINKER_SCRIPT} \
     -Wl,--gc-sections \
     -specs=nosys.specs -specs=nano.specs")

# 收集源文件和头文件
file(GLOB_RECURSE SOURCES "code/*.c" "driver/*.c" "libraries/*.c" "code/*.cpp" "driver/*.cpp" "libraries/*.cpp")
include_directories(code driver libraries)

# 设置启动文件
set(STARTUP_ASM "${CMAKE_SOURCE_DIR}/driver/startup_stm32f10x_md.s")

# 添加宏定义
add_definitions(
    -DSTM32F10X_MD
    -DUSE_STDPERIPH_DRIVER
)

#生成对应文件
add_executable(firmware.elf ${SOURCES} ${STARTUP_ASM})
add_custom_target(hex ALL
    COMMAND ${CMAKE_OBJCOPY} -O ihex firmware.elf firmware.hex
    DEPENDS firmware.elf)

add_custom_target(bin ALL
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
    DEPENDS firmware.elf)
