cmake_minimum_required(VERSION 3.20)
# 定义模块名称
project(sdk C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

# 工具链设置
# set(TOOLPATH /home/MrTan/Documents/ch32_toolchain/riscv_gcc12/bin/riscv-wch-elf-)

# if(WIN32)
#     MESSAGE(STATUS "Now is windows!")
#     set(CMAKE_C_COMPILER ${TOOLPATH}gcc.exe)
#     set(CMAKE_CXX_COMPILER ${TOOLPATH}g++.exe)
#     set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc.exe)
#     set(CMAKE_AR ${TOOLPATH}ar.exe)
#     set(CMAKE_OBJCOPY ${TOOLPATH}objcopy.exe)
#     set(CMAKE_OBJDUMP ${TOOLPATH}objdump.exe)
#     set(SIZE ${TOOLPATH}size.exe)
# elseif(UNIX)
#     MESSAGE(STATUS "Now is UNIX-like OS!")
#     set(CMAKE_C_COMPILER ${TOOLPATH}gcc)
#     set(CMAKE_CXX_COMPILER ${TOOLPATH}g++)
#     set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc)
#     set(CMAKE_AR ${TOOLPATH}ar)
#     set(CMAKE_OBJCOPY ${TOOLPATH}objcopy)
#     set(CMAKE_OBJDUMP ${TOOLPATH}objdump)
#     set(SIZE ${TOOLPATH}size)
# else()
#     MESSAGE(STATUS "Unsupported system!")
# endif()

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ./*.c ./*.h)
file(GLOB_RECURSE includes CONFIGURE_DEPENDS ./*.h)


# 添加源文件并创建库
add_library(sdk ${srcs})
# 添加头文件目录
target_include_directories(sdk PUBLIC ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Core
    ${PROJECT_SOURCE_DIR}/Peripheral/inc
    ${PROJECT_SOURCE_DIR}/Peripheral
)

# 如果需要链接其他库，可以使用 target_link_libraries
# target_link_libraries(${MODULE_NAME} PRIVATE some_other_library)