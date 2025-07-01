cmake_minimum_required(VERSION 3.20)
# 定义模块名称
project(sdk C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ./*.c ./*.h)
file(GLOB_RECURSE includes CONFIGURE_DEPENDS ./*.h)


# 添加源文件并创建库
add_library(sdk ${srcs})
# 添加头文件目录
target_include_directories(sdk PUBLIC ${PROJECT_SOURCE_DIR})

# 如果需要链接其他库，可以使用 target_link_libraries
# target_link_libraries(${MODULE_NAME} PRIVATE some_other_library)