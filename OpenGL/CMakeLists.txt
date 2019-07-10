cmake_minimum_required(VERSION 3.14)
project(cg)

set(CMAKE_CXX_STANDARD 17)

find_library(glut GLUT)
find_library(opengL OpenGL)

add_executable(main main.cpp index.h basic.cpp polygon.cpp color.cpp)
target_link_libraries(main
        "-framework GLUT"
        "-framework OpenGL")