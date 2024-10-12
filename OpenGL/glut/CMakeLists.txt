cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

add_executable(glut_app
        main.cpp
        index.h
        basic.cpp
        polygon.cpp
        color.cpp
        matrix.cpp)

target_include_directories(glut_app PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(glut_app ${OPENGL_gl_LIBRARY})
target_link_libraries(glut_app
        "-framework GLUT")