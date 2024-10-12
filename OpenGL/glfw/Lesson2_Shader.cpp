/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-08
* Time:   16:43
* Life with Passion, Code with Creativity.
* </pre>
*/
#include <iostream>
#include <cmath>
#include "index.h"
#include "glhelper.h"


constexpr auto vertexShader = R"(
#version 330 core

layout (location = 0) in vec3 position; // 位置变量的属性位置值为 0
layout (location = 1) in vec3 color;    // 颜色变量的属性位置值为 1
uniform float xTrans;

out vec3 ourColor; // output color to fragment shader

void main() {
    gl_Position = vec4(position.x + xTrans, position.y, position.z, 1.0f);
    ourColor = color;
}
)";

constexpr auto fragmentShader = R"(
#version 330 core
in vec3 ourColor; // from vertex shader
out vec4 color;

void main() {
    // RGBA
    color = vec4(ourColor, 1.0f);
}

)";

class ShaderRenderer : public Renderer {
private:
    gl::ShaderMachine<false, GLuint> shaderMachine;
//    GLfloat vertices[18]{
//            // 位置              // 颜色
//            0.5f, -0.5f, 0.0f,  /**/ 1.0f, 0.0f, 0.0f,   // 右下
//            -0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f,   // 左下
//            0.0f, 0.5f, 0.0f,   /**/ 0.0f, 0.0f, 1.0f    // 顶部
//    };
    // upside down
        GLfloat vertices[18]{
            // 位置              // 颜色
            0.0f, -0.5f, 0.0f,  /**/ 1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, 0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f,   // 左下
            0.5f, 0.5f, 0.0f,   /**/ 0.0f, 0.0f, 1.0f    // 顶部
    };
public:
    ShaderRenderer() :
            shaderMachine(
                    vertexShader,
                    fragmentShader,
                    [](auto &p, auto &u) { u = p.getUniformLocation("xTrans"); }) {
        if (!shaderMachine.success()) {
            std::cerr << "compile shader failed " << shaderMachine.getCompileLog() << std::endl;
            return;
        }

        auto vaoScope = shaderMachine.useVertexArrayObject();
        // operations on the VAO
        {
            {
                auto vboScope = shaderMachine.useVertexBufferObject();
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
                        6 * sizeof(GLfloat),
                        // offset?
                        nullptr
                );
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(
                        // location in shader
                        1,
                        // size
                        3,
                        // type
                        GL_FLOAT,
                        // normalize to [-1, 1]
                        GL_FALSE,
                        // stride (length of each element)
                        6 * sizeof(GLfloat),
                        // offset?
                        (void *) (3 * sizeof(GLfloat))
                );
                glEnableVertexAttribArray(1);
            }
        }
    }

    ~ShaderRenderer() override = default;

    void render(float width, float height) override {
        gl::Scope p(shaderMachine);

        glUniform1f(shaderMachine.extra, 0.4f);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

};

Renderer *makeShaderRenderer() {
    return new ShaderRenderer();
}