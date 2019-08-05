cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)

add_executable(myapp
        test.cpp)
target_include_directories(myapp PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(myapp glfw ${OPENGL_gl_LIBRARY})


