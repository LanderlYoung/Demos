cmake_minimum_required(VERSION 3.14)
project(cg)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_subdirectory(glut)
add_subdirectory(glfw)
