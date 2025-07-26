cmake_minimum_required(VERSION 3.16)
project(stm32f10x_stdlib C ASM)

#-----------------------------------------------------------
# 1. 强制使用交叉工具链
#-----------------------------------------------------------
set(CMAKE_SYSTEM_NAME          Generic)
set(CMAKE_SYSTEM_PROCESSOR     arm)

set(CMAKE_C_COMPILER           arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER         arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER         arm-none-eabi-gcc)
set(CMAKE_OBJCOPY              arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP              arm-none-eabi-objdump)

# 让 find_* 只搜索目标系统
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#-----------------------------------------------------------
# 2. MCU 与通用编译选项
#-----------------------------------------------------------
set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb")

set(CMAKE_C_FLAGS
    "${MCU_FLAGS} \
     -ffunction-sections \
     -fdata-sections \
     -Wall -Wextra \
     -g -O2 \
     -std=c99")

set(CMAKE_ASM_FLAGS "${MCU_FLAGS}")

#-----------------------------------------------------------
# 3. 链接脚本与链接器选项
#-----------------------------------------------------------
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/driver/stm32_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS
    "${MCU_FLAGS} \
     -T${LINKER_SCRIPT} \
     -Wl,--gc-sections \
     -specs=nosys.specs -specs=nano.specs")

#-----------------------------------------------------------
# 4. 源文件收集（排除 core_cm3.c）
#-----------------------------------------------------------
file(GLOB_RECURSE SOURCES
     "code/*.c"
     "driver/*.c"
     "libraries/*.c")

list(REMOVE_ITEM SOURCES
     "${CMAKE_SOURCE_DIR}/driver/core_cm3.c")   # 必须剔除

set(STARTUP_ASM "${CMAKE_SOURCE_DIR}/driver/startup_stm32f10x_md.s")

#-----------------------------------------------------------
# 5. 头文件路径
#-----------------------------------------------------------
include_directories(code driver libraries)

#-----------------------------------------------------------
# 6. 可执行文件
#-----------------------------------------------------------
add_executable(firmware.elf ${SOURCES} ${STARTUP_ASM})

#-----------------------------------------------------------
# 7. 宏定义
#-----------------------------------------------------------
target_compile_definitions(firmware.elf PRIVATE
    STM32F10X_MD
    USE_STDPERIPH_DRIVER)

#-----------------------------------------------------------
# 8. 生成 hex / bin
#-----------------------------------------------------------
add_custom_target(hex ALL
    COMMAND ${CMAKE_OBJCOPY} -O ihex firmware.elf firmware.hex
    DEPENDS firmware.elf)

add_custom_target(bin ALL
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
    DEPENDS firmware.elf)
