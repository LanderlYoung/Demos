/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-05
* Time:   13:04
* Life with Passion, Code with Creativity.
* </pre>
*/

#include <string_view>
#include <iostream>
#include "index.h"
#include "glhelper.h"

constexpr auto vertexShader = R"(
#version 330 core

layout (location=0)
in vec3 position;

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
}
)";

constexpr auto fragmentShader = R"(
#version 330 core
out vec4 color;

void main() {
    // RGBA
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}

)";

class MyTriangleRenderer : public Renderer {
private:
    gl::ShaderProgram shaderProgram;
    GLuint VBO = 0;
    GLuint VAO = 0;

public:
    MyTriangleRenderer() : shaderProgram(vertexShader, fragmentShader) {
        if (!shaderProgram.success()) {
            std::cerr << "compile shader failed " << shaderProgram.getCompileLog() << std::endl;
            return;
        }

        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        // operations on the VAO
        {
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Set up vertex data (and buffer(s)) and attribute pointers
            GLfloat vertices[] = {
                    -0.5f, -0.5f, 0.0f, // Left
                    0.5f, -0.5f, 0.0f, // Right
                    0.0f, 0.5f, 0.0f  // Top
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            //https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/04%20Hello%20Triangle/
            glVertexAttribPointer(
                    // location in shader
                    0,
                    // size
                    3,
                    // type
                    GL_FLOAT,
                    // normalize to [-1, 1]
                    GL_FALSE,
                    // stride (length of each element)
                    3 * sizeof(GLfloat),
                    // offset?
                    nullptr
            );
            glEnableVertexAttribArray(0);

            // release bound VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // release bound VAO
            glBindVertexArray(0);
        }
    }

    ~MyTriangleRenderer() override {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void render() override {
        auto scope = shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // release bound array
        glBindVertexArray(0);
    }

};

Renderer *makeMyTriangleRenderer() {
    return new MyTriangleRenderer();
}