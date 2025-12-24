# ============================================================================
# CMake 配置文件 - STM32F429 + FreeRTOS 项目
# ============================================================================
# 本文件用于配置基于 STM32F429 芯片的嵌入式项目，集成了 FreeRTOS 实时操作系统
# 使用 ARM GCC 工具链进行交叉编译
# ============================================================================

# ----------------------------------------------------------------------------
# 基础项目配置
# ----------------------------------------------------------------------------
# 指定 CMake 最低版本需求（确保使用的 CMake 功能可用）

# @author 王广平 
# @author Yukikaze
cmake_minimum_required(VERSION 3.28.1)

# 定义项目名称（生成的可执行文件将使用此名称）
project(template)

# 设置 C 语言标准为 C23，并强制要求编译器支持
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置 C++ 语言标准为 C++23，并强制要求编译器支持
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用汇编语言支持（用于编译启动文件 startup_stm32f429xx.s）
ENABLE_LANGUAGE(ASM)

# 芯片型号定义 (STM32F429/439系列)
add_compile_definitions(STM32F429_439xx)

# 让标准外设库包含用户配置（stm32f4xx_conf.h）
add_compile_definitions(USE_STDPERIPH_DRIVER)

# ----------------------------------------------------------------------------
# 芯片架构配置
# ----------------------------------------------------------------------------
# 指定目标 MCU 的 Cortex 内核类型
set(MCU cortex-m4)

# 配置浮点单元（FPU）选项
# -mfpu=fpv4-sp-d16: 使用 FPv4 单精度浮点指令集，支持 16 个双精度寄存器
# -mfloat-abi=hard: 使用硬件浮点运算（提高浮点性能）
set(FPU_FLAGS "-mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# ----------------------------------------------------------------------------
# 目录结构配置
# ----------------------------------------------------------------------------
# 基础路径：MCU 相关文件根目录
set(MCU_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../mcu)

# 板级支持包（BSP）目录：包含外设驱动（如 LED、按键等）
set(BSP_DIR ${MCU_DIR}/bsp)

# 用户代码目录：包含 main.c 和中断处理函数
set(USER_DIR ${MCU_DIR}/user)

# 库文件目录：包含 CMSIS 和 HAL 驱动库
set(LIB_DIR ${MCU_DIR}/Libraries)

# 链接脚本目录：包含内存布局定义文件
set(LD_DIR ${LIB_DIR}/ld)

# 扩展库目录：用户自定义的工具函数库
set(LIBX_DIR ${MCU_DIR}/libx)

# 应用层目录：业务逻辑代码
set(APP_DIR ${MCU_DIR}/app)

# ----------------------------------------------------------------------------
# FreeRTOS 配置
# ----------------------------------------------------------------------------
# RTOS 组根目录
set(CRM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../crm)

# FreeRTOS 根目录
set(FREERTOS_DIR ${CRM_DIR}/freeRTOS)

# FreeRTOS 头文件目录：包含 FreeRTOS.h、task.h 等核心头文件
set(FRTOS_INC_DIR ${FREERTOS_DIR}/include)

# FreeRTOS 可移植层目录：包含不同编译器和架构的适配代码
set(FRTOS_PORT_DIR ${FREERTOS_DIR}/portable)

# FreeRTOS 源文件目录：包含任务管理、队列、信号量等核心实现
set(FRTOS_SRC_DIR ${FREERTOS_DIR}/src)

# 内存管理目录：包含 heap_1.c ~ heap_5.c 等内存分配方案
set(MEMMANG_DIR ${FRTOS_PORT_DIR}/MemMang)

# Cortex-M4F 移植层目录：针对 ARM Cortex-M4F 的 FreeRTOS 适配代码
set(ARM_CM4F_DIR ${FRTOS_PORT_DIR}/GCC/ARM_CM4F)

# ----------------------------------------------------------------------------
# 编译和链接选项
# ----------------------------------------------------------------------------
# C 编译器标志：
# -mcpu=${MCU}: 指定目标 CPU 架构
# -mthumb: 使用 Thumb 指令集（节省代码空间）
# ${FPU_FLAGS}: 浮点单元配置
# -O2: 优化级别 2（平衡性能和代码大小）
# -ffunction-sections: 将每个函数放入独立段（便于链接器删除未使用代码）
# -fdata-sections: 将每个数据放入独立段
# -w: 禁用所有警告（只显示错误）
# -g: 生成调试信息
set(CMAKE_C_FLAGS "-mcpu=${MCU} -mthumb ${FPU_FLAGS} -O2 -ffunction-sections -fdata-sections -w -g")

# 汇编编译器标志：禁用警告
set(CMAKE_ASM_FLAGS "-mcpu=${MCU} -mthumb ${FPU_FLAGS} -w")

# 链接脚本：定义 Flash 和 RAM 的内存布局
set(LINKER_SCRIPT "${LD_DIR}/STM32F429IGTX_FLASH.ld")

# 链接器标志：
# -T\"${LINKER_SCRIPT}\": 指定链接脚本（使用引号处理路径中的空格）
# -Wl,--gc-sections: 删除未使用的代码段和数据段（减小最终固件大小）
# -static: 静态链接
set(CMAKE_EXE_LINKER_FLAGS "-T\"${LINKER_SCRIPT}\" -Wl,--gc-sections -static")

# 启动文件：芯片启动汇编代码（初始化堆栈、复制数据段、跳转到 main）
set(STARTUP_FILE ${LIB_DIR}/CMSIS/startup_stm32f429_439xx.s)

# ----------------------------------------------------------------------------
# 头文件包含目录配置
# ----------------------------------------------------------------------------
# 自动收集各模块的头文件目录（避免硬编码）
# 规则：自动扫描每个模块下的 Inc 或 inc 子目录，以及模块根目录本身

# BSP 板级支持包：扫描 bsp/*/Inc 和 bsp/**/Inc（支持多层嵌套）
file(GLOB_RECURSE BSP_INCLUDE_DIRS LIST_DIRECTORIES true ${BSP_DIR}/*/Inc ${BSP_DIR}/*/*/Inc)

# APP 应用层：扫描 app/*/Inc 和 app/**/Inc
file(GLOB_RECURSE APP_INCLUDE_DIRS LIST_DIRECTORIES true ${APP_DIR}/*/Inc ${APP_DIR}/*/*/Inc)

# 扩展模块通用扫描（支持 middleware等同级目录）
# 扫描 mcu 目录下所有子目录中的 Inc 目录
file(GLOB_RECURSE MCU_MODULE_INCLUDE_DIRS LIST_DIRECTORIES true 
    ${MCU_DIR}/middleware/Inc
    ${MCU_DIR}/middleware/*/Inc
)

# 包含目录列表
include_directories(
    # ========== 库文件头文件 ==========
    # CMSIS 标准接口：Cortex-M 微控制器软件接口标准
    ${LIB_DIR}/CMSIS
    
    # STM32 标准库头文件
    ${LIB_DIR}/STM32F4xx_StdPeriph_Driver
    
    # ========== FreeRTOS 头文件 ==========
    # FreeRTOS 核心头文件目录
    ${FRTOS_INC_DIR}
    
    # FreeRTOS 针对 ARM Cortex-M4F 的移植层头文件
    ${ARM_CM4F_DIR}

    # ========== 用户代码头文件 ==========
    # 用户主程序头文件
    ${USER_DIR}
    
    # BSP 板级支持包头文件（自动包含所有子模块的 Inc 目录）
    ${BSP_INCLUDE_DIRS}
    
    # 扩展工具库头文件
    ${LIBX_DIR}
    
    # APP 应用层头文件（自动包含所有子模块的 Inc 目录）
    ${APP_INCLUDE_DIRS}
    
    # 扩展模块头文件（wifi、modbus 等）
    ${MCU_MODULE_INCLUDE_DIRS}
)


# ----------------------------------------------------------------------------
# 源文件收集
# ----------------------------------------------------------------------------
# 使用递归模式自动收集所有 .c 源文件
# GLOB_RECURSE 会递归搜索子目录，确保所有源文件都被包含
file(GLOB_RECURSE SRC_FILES
    # ========== 库文件源码 ==========
    # CMSIS 系统初始化文件（system_stm32f4xx.c）
    ${LIB_DIR}/CMSIS/*.c
    
    # STM32 标准库实现文件（各种外设驱动）
    ${LIB_DIR}/STM32F4xx_StdPeriph_Driver/*.c
    
    # ========== FreeRTOS 源码 ==========
    # ARM Cortex-M4F 移植层实现（port.c）
    ${ARM_CM4F_DIR}/*.c
    
    # FreeRTOS 内存管理实现（heap_x.c）
    ${MEMMANG_DIR}/*.c
    
    # FreeRTOS 核心源码（tasks.c, queue.c, timers.c 等）
    ${FRTOS_SRC_DIR}/*.c

    # ========== 用户代码 ==========
    # 用户主程序和中断处理（main.c, stm32f4xx_it.c）
    ${USER_DIR}/*.c
    
    # BSP 板级支持包实现（递归包含所有子模块的 .c 文件）
    ${BSP_DIR}/**/*.c
    
    # 扩展工具库实现
    ${LIBX_DIR}/*.c
    
    # APP 应用层业务逻辑实现（递归包含所有子目录）
    ${APP_DIR}/**/*.c
    
    # WiFi 模块实现（如果存在）
    ${MCU_DIR}/wifi/**/*.c
    
    # Modbus 协议实现（如果存在）
    ${MCU_DIR}/modbus/**/*.c
)

# ----------------------------------------------------------------------------
# 库文件过滤
# ----------------------------------------------------------------------------
# 移除不需要的源文件
list(REMOVE_ITEM SRC_FILES
    ${LIB_DIR}/STM32F4xx_StdPeriph_Driver/stm32f4xx_fsmc.c
)

# ----------------------------------------------------------------------------
# 目标文件生成
# ----------------------------------------------------------------------------
# 生成 ELF 格式的可执行文件
# ELF 文件包含调试信息和符号表，用于调试和烧录
add_executable(${PROJECT_NAME}.elf ${SRC_FILES} ${STARTUP_FILE})

# ----------------------------------------------------------------------------
# 固件格式转换
# ----------------------------------------------------------------------------
# 在 ELF 文件生成后，自动转换为其他常用格式
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    # 生成 Intel HEX 格式（文本格式，常用于烧录工具）
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    
    # 生成二进制 BIN 格式（纯二进制，体积最小，适合 OTA 升级）
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    
    COMMENT "正在生成 HEX 和 BIN 固件文件..."
)