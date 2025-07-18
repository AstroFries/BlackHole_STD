add_executable(IO_TEST test/io_test.cpp)
add_executable(WINDOW_TEST test/window_test.cpp)
add_executable(GEODESIC_TEST test/geodesic_test.cpp)
add_executable(VOLUME_RENDER_TEST  test/volume_render_test.cpp)
add_executable(SWARZSCHILD_BH Schwarzschild.cpp)
#add_executable(SWZ_BH_REALTIME Swz_Realtime_OMP.cpp)
add_executable(SWZ_BH_REALTIME Swz_Realtime.cpp)
add_executable(BH_REALTIME_GL Swz_Realtime_GL.cpp)
add_executable(BLOOM_TEST test/bloom_test.cpp)
add_executable(IMGUI_TEST test/imgui_test.cpp)
add_executable(GEODESIC_AUTO_TEST test/geodesic_auto_test.cpp)

target_link_libraries(GEODESIC_TEST PRIVATE GR_LIB)
target_link_libraries(VOLUME_RENDER_TEST PRIVATE GR_LIB)
target_link_libraries(WINDOW_TEST PRIVATE GR_LIB)
target_link_libraries(SWARZSCHILD_BH PRIVATE GR_LIB)
target_link_libraries(SWZ_BH_REALTIME PRIVATE GR_LIB)
target_link_libraries(BH_REALTIME_GL PRIVATE GR_LIB)
target_link_libraries(BLOOM_TEST PRIVATE GR_LIB)
target_link_libraries(GEODESIC_AUTO_TEST PRIVATE GR_LIB)

add_subdirectory(shaders)

set(SHADERS_DIR "${PROJECT_SOURCE_DIR}/src/shaders")
add_definitions(-DSHADERS_DIR="${SHADERS_DIR}")