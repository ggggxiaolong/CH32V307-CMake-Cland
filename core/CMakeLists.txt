cmake_minimum_required(VERSION 3.20)
# 定义模块名称
project(core CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

# # 工具链设置
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

add_definitions(-DCH32V307=1)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS *.cpp *.hpp)
# file(GLOB_RECURSE includes CONFIGURE_DEPENDS *.hpp)

# 添加源文件并创建库
add_library(core ${srcs})
message(STATUS "root: ${PROJECT_SOURCE_DIR}")
# 添加头文件目录
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/arch/arm
    ${PROJECT_SOURCE_DIR}/arch/loongarch
    ${PROJECT_SOURCE_DIR}/arch/riscv
    ${PROJECT_SOURCE_DIR}/arch/xtensa
    ${PROJECT_SOURCE_DIR}/buffer/pingpongbuf
    ${PROJECT_SOURCE_DIR}/buffer/ringbuf
    ${PROJECT_SOURCE_DIR}/buffer/stack
    ${PROJECT_SOURCE_DIR}/buffer
    ${PROJECT_SOURCE_DIR}/clock
    ${PROJECT_SOURCE_DIR}/constants
    ${PROJECT_SOURCE_DIR}/debug
    ${PROJECT_SOURCE_DIR}/defines
    ${PROJECT_SOURCE_DIR}/fp
    ${PROJECT_SOURCE_DIR}/io
    ${PROJECT_SOURCE_DIR}/math
    ${PROJECT_SOURCE_DIR}/math/float
    ${PROJECT_SOURCE_DIR}/math/fast
    ${PROJECT_SOURCE_DIR}/math/int
    ${PROJECT_SOURCE_DIR}/math/iq/simulates
    ${PROJECT_SOURCE_DIR}/math/iq/universal
    ${PROJECT_SOURCE_DIR}/math/iq
    ${PROJECT_SOURCE_DIR}/os/tasker
    ${PROJECT_SOURCE_DIR}/polymorphism
    ${PROJECT_SOURCE_DIR}/stream
    ${PROJECT_SOURCE_DIR}/string
    ${PROJECT_SOURCE_DIR}/string/encoding
    ${PROJECT_SOURCE_DIR}/string/fp
    ${PROJECT_SOURCE_DIR}/string/utils
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/utils/details
    ${PROJECT_SOURCE_DIR}/utils/setget
    ${PROJECT_SOURCE_DIR}/utils/typetraits
)

# 如果需要链接其他库，可以使用 target_link_libraries
# target_link_libraries(${MODULE_NAME} PRIVATE some_other_library)