# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_C_STANDARD 99)
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.hpp)
# file(GLOB_RECURSE includes CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.h)

# 添加源文件并创建库
add_library(hal STATIC ${srcs})
# target_compile_definitions(core PUBLIC -DCH32V307)
# 添加头文件目录
target_include_directories(hal PUBLIC ${PROJECT_SOURCE_DIR})
# 模块相互依赖关系
target_link_libraries(hal PRIVATE sdk core)
