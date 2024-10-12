cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(glfw_app
        utils.h
        glhelper.h
        glhelper.cpp
        glfw_main.cpp
        Lesson1_Triangle.cpp
        Lesson1.1_Rectangle.cpp
        Lesson2_Shader.cpp
        lesson3_Texture.cpp

        ../ext/lodepng/lodepng.h
        ../ext/lodepng/lodepng.cpp
        )
target_include_directories(glfw_app PUBLIC
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR})
target_link_libraries(glfw_app
        glfw
        ${GLEW_LIBRARIES}
        ${OPENGL_gl_LIBRARY})


