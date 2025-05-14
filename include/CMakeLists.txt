file(GLOB_RECURSE GR_SOURCES "*.cpp")
file(GLOB_RECURSE GR_HEADERS "*.h")

# 添加一个库目标
add_library(GR_LIB ${GR_SOURCES} ${GR_HEADERS})

target_include_directories(GR_LIB PUBLIC ${CMAKE_SOURCE_DIR}/external/Eigen)