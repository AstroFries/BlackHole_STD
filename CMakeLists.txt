cmake_minimum_required(VERSION 3.20)

project(
  "BlackHole_Observer"
  VERSION 1.0
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(FRAMEWORK2D_DIR "${PROJECT_SOURCE_DIR}")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/external")
set(BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_DIR "${PROJECT_SOURCE_DIR}/libs")

include_directories(${INCLUDE_DIR})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(external)

#Find OpenGL 
find_package(OpenGL REQUIRED)
if (NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()

message(STATUS "OpenGL found: ${OPENGL_INCLUDE_DIR}")
include_directories(${OPENGL_INCLUDE_DIR})
message(STATUS "OpenGL found: ${OPENGL_LIBRARIES}")
link_directories(${OPENGL_LIBRARIES})
message(STATUS "OpenGL found: ${OPENGL_gl_LIBRARY}")
link_directories(${OPENGL_gl_LIBRARY})


#Eigen
set(EIGEN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/Eigen")
target_include_directories(GEODESIC_TEST PRIVATE ${EIGEN_INCLUDE_DIR})
target_include_directories(VOLUME_RENDER_TEST PRIVATE ${EIGEN_INCLUDE_DIR})

#Link GLFW to the project
#target_link_libraries(IO_TEST OpenGL::GL glfw)
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

target_include_directories(GR_LIB PUBLIC OpenGL::GL glfw glad)
target_link_libraries(GR_LIB PUBLIC OpenGL::GL glfw glad)
target_link_libraries(WINDOW_TEST PRIVATE OpenGL::GL glfw glad)
target_link_libraries(VOLUME_RENDER_TEST PRIVATE OpenGL::GL glfw glad)

#--------------------------
#Python 
#set(Python3_FIND_STRATEGY LOCATION)
#set(Python3_ROOT_DIR "C:/Users/12989/miniconda3/envs/forplt")
#set(Python3_ROOT_DIR "C:/Users/12989/miniconda3")
#set(Python3_FIND_VERSION_RANGE "3.6...<3.9")
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_include_directories(GEODESIC_TEST PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/external/matplotlibcpp
)
target_link_libraries(GEODESIC_TEST PRIVATE
    ${Python3_LIBRARIES}  
)
find_package(Python3 COMPONENTS NumPy REQUIRED)
target_include_directories(GEODESIC_TEST PRIVATE ${Python3_NumPy_INCLUDE_DIRS})
message(STATUS "Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARIES: ${Python3_LIBRARIES}")