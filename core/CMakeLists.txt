# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.hpp)
# file(GLOB_RECURSE includes CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.hpp)

# 添加源文件并创建库
add_library(core ${srcs})
target_compile_definitions(core PUBLIC -DCH32V307 -DCH32V30X -DUSE_CH32V3_STD_SDK)
# 添加头文件目录
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR})
# 模块相互依赖关系
target_link_libraries(core PRIVATE sdk hal)
