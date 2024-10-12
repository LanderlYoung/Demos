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

class TriangleRenderer : public Renderer {
private:
    gl::ShaderProgram shaderProgram;
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLfloat vertices[9] = {
            -0.5f, -0.5f, 0.0f, // Left
            0.5f, -0.5f, 0.0f,  // Right
            0.0f, 0.5f, 0.0f    // Top
    };
public:
    TriangleRenderer() : shaderProgram(vertexShader, fragmentShader) {
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
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

            //https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/04%20Hello%20Triangle/
            glVertexAttribPointer(
                    // 第一个参数指定我们要配置的顶点属性。
                    // 还记得我们在顶点着色器中使用layout(location = 0)
                    // 定义了position顶点属性的位置值(Location)吗？
                    // 它可以把顶点属性的位置值设置为0。
                    // 因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
                    0,
                    // 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
                    3,
                    // 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
                    GL_FLOAT,
                    // 下个参数定义我们是否希望数据被标准化(Normalize)。
                    // 如果我们设置为GL_TRUE，所有数据都会被映射到0
                    // （对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
                    GL_FALSE,
                    // 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
                    // 由于下个组位置数据在3个GLfloat之后，我们把步长设置为3 * sizeof(GLfloat)。
                    // 要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
                    // 我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
                    // 一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，
                    // 我们在后面会看到更多的例子(译注: 这个参数的意思简单说就是从这个属性
                    // 第二次出现的地方到整个数组0位置之间有多少字节)。
                    3 * sizeof(GLfloat),
                    // 最后一个参数的类型是GLvoid*，所以需要我们进行这个奇怪的强制类型转换。
                    // 它表示位置数据在缓冲中起始位置的偏移量(Offset)。
                    // 由于位置数据在数组的开头，所以这里是0。
                    // 我们会在后面详细解释这个参数。
                    nullptr
            );
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // release bound VBO

            // release bound VAO
            glBindVertexArray(0);
        }
    }

    ~TriangleRenderer() override {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void step() {
        if (vertices[0] > 1.0f) {
            vertices[0] = -1.0f;
            vertices[1] = -1.0f;

            vertices[3] = 0.0f;
            vertices[4] = -1.0f;

            vertices[6] = -0.5f;
            vertices[7] = -0.0f;
        } else {
            for (auto i = 0; i < 3; i++) {
                auto stepLen = 0.0005f;
                vertices[i * 3] += stepLen;
                vertices[i * 3 + 1] += stepLen;
            }
        }
    }

    void render() override {
        auto scope = shaderProgram.use();
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        step();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // release bound array
        glBindVertexArray(0);
    }

};

Renderer *makeTriangleRenderer() {
    return new TriangleRenderer();
}
