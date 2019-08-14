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

class RectangleRenderer : public Renderer {
private:
    gl::ShaderProgram shaderProgram;
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLfloat vertices[12] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };

    GLuint indices[6] = { // 注意索引从0开始!
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
public:
    RectangleRenderer() : shaderProgram(vertexShader, fragmentShader) {
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
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glEnableVertexAttribArray(0);

            {
                // create index for VBO
                glGenBuffers(1, &EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            // release bound VBO

            // release bound VAO
            glBindVertexArray(0);
        }
    }

    ~RectangleRenderer() override {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void render() override {
        auto scope = shaderProgram.use();
        glBindVertexArray(VAO);

        {
            // draw with EBO

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        {
            // draw with VBO

            // glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // glDrawArrays(GL_TRIANGLES, 0, 4);
        }

        glBindVertexArray(0);
    }

};

Renderer *makeRectangleRenderer() {
    return new RectangleRenderer();
}
