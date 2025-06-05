set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.20)

# 工具链设置
set(TOOLPATH /home/MrTan/Documents/ch32_toolchain/riscv_gcc12/bin/riscv-wch-elf-)

if(WIN32)
    MESSAGE(STATUS "Now is windows!")
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc.exe)
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++.exe)
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc.exe)
    set(CMAKE_AR ${TOOLPATH}ar.exe)
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy.exe)
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump.exe)
    set(SIZE ${TOOLPATH}size.exe)
elseif(UNIX)
    MESSAGE(STATUS "Now is UNIX-like OS!")
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++)
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc)
    set(CMAKE_AR ${TOOLPATH}ar)
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy)
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump)
    set(SIZE ${TOOLPATH}size)
else()
    MESSAGE(STATUS "Unsupported system!")
endif()

# 项目设置
project(ch32v307-ninja C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

# 编译参数 一般不用改
add_compile_options(-march=rv32imafc -mabi=ilp32f -mcmodel=medany -msmall-data-limit=8 -mno-save-restore)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -fsingle-precision-constant)

# 编译等级
add_compile_options(-O2)

# 编译信息等级
add_compile_options(-Wall)


# 添加子目录以包含模块
add_subdirectory(core)
add_subdirectory(sdk)

# # 将模块链接到目标可执行文件或库
# target_link_libraries(${PROJECT_NAME}.elf PRIVATE core sdk)
# 头文件路径
include_directories(APP
    Debug
    sdk/Peripheral/inc)

# 宏定义
# add_definitions(-DDEBUG=1)

# 源码文件
file(GLOB_RECURSE SOURCES
    "APP/*.c"
    "APP/*.cpp"
    "sdk/Startup/startup_ch32v30x_D8C.S"
    # "sdk/Core/core_riscv.c"
    # "Debug/debug.c"
    # "sdk/Peripheral/src/*.c"
    # "sdk/*.c"
    # "core/debug/*.cpp"
    # "core/stream/*.cpp"
)

# 链接参数
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/sdk/Ld/Link.ld)
add_link_options(
    -march=rv32imafc -mabi=ilp32f
    -nostartfiles
    -Xlinker --gc-sections
    -Wl,--print-memory-usage
    -Wl,-Map,${PROJECT_NAME}.map
    --specs=nano.specs
    --specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

# 编译可执行文件
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf PRIVATE core)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE sdk)

target_compile_definitions(${PROJECT_NAME}.elf PUBLIC CH32V307=1)

# 链接静态库
# target_link_libraries(${PROJECT_NAME}.elf printfloat)

# 输出hex和bin
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(LST_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.lst)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LST_FILE}
    COMMAND ${SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
)
