add_executable(IO_TEST test/io_test.cpp)
add_executable(WINDOW_TEST test/window_test.cpp)
add_executable(GEODESIC_TEST test/geodesic_test.cpp)
add_executable(VOLUME_RENDER_TEST  test/volume_render_test.cpp)

target_link_libraries(GEODESIC_TEST PRIVATE GR_LIB)
target_link_libraries(VOLUME_RENDER_TEST PRIVATE GR_LIB)
target_link_libraries(WINDOW_TEST PRIVATE GR_LIB)

add_subdirectory(shaders)

set(SHADERS_DIR "${PROJECT_SOURCE_DIR}/src/shaders")
add_definitions(-DSHADERS_DIR="${SHADERS_DIR}")