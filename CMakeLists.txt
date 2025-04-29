cmake_minimum_required(VERSION 3.14)

project(
  "BlackHole_Observer"
  VERSION 1.0
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
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


#Add GLFW Submodule

#Link GLFW to the project
#target_link_libraries(IO_TEST OpenGL::GL glfw)
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

target_link_libraries(WINDOW_TEST OpenGL::GL glfw glad)