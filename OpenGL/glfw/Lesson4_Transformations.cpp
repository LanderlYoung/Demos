/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-14
* Time:   16:29
* Life with Passion, Code with Creativity.
* </pre>
*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string_view>
#include <iostream>
#include "index.h"
#include "glhelper.h"
#include "../ext/lodepng/lodepng.h"

constexpr auto vertexShader = R"(
#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
layout (location=2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(position, 1.0f);
    ourColor = color;
    TexCoord = texCoord;
}
)";

constexpr auto fragmentShader = R"(
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

//uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float mixValue;

void main() {
    // color = texture(ourTexture, TexCoord);

    // fix inverse texture picture issue
    vec2 inv = vec2(TexCoord.x, 1.0f -TexCoord.y);

    // blend texture
    // 1-0.2 first + 0.2 second
    // color = mix(texture(ourTexture1, inv), texture(ourTexture2, inv), 0.2);
    color = mix(texture(ourTexture1, inv), texture(ourTexture2, inv), mixValue);
}

)";

class TransformationsRenderer : public Renderer {
private:
    struct UniformLocation {
        GLuint texture1;
        GLuint texture2;
        GLuint mixValue;
        GLuint transform;
    };

    float mixValue = 0.2;

    gl::ShaderMachine<true, UniformLocation> shaderMachine;
    GLuint texture1 = 0;
    GLuint texture2 = 0;
    GLfloat vertices[32]{
            //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
    };

    GLuint indices[6]{ // 注意索引从0开始!
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };

    static unsigned loadImage(const std::string &name, std::vector<unsigned char> &image, unsigned &width, unsigned &height) {
        std::string base = __FILE__;
        auto file = base.substr(0, base.rfind('/') + 1) + "assets/" + name;
        return lodepng::decode(image, width, height, file, LodePNGColorType::LCT_RGB);
    }

public:
    TransformationsRenderer() : shaderMachine(vertexShader, fragmentShader, [](auto &p, auto &u) {
        u.texture1 = p.getUniformLocation("ourTexture1");
        u.texture2 = p.getUniformLocation("ourTexture2");
        u.mixValue = p.getUniformLocation("mixValue");
        u.transform = p.getUniformLocation("transform");
    }) {
        if (!shaderMachine.success()) {
            std::cerr << "compile shader failed " << shaderMachine.getCompileLog() << std::endl;
            return;
        }

        prepareVertexData();
        glCheckError();

        prepareTexture();
        glCheckError();
    }

    void prepareVertexData() {
        auto vaoScope = shaderMachine.useVertexArrayObject();
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
                8 * sizeof(GLfloat),
                // offset?
                nullptr
        );
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        {
            // create index for VBO
            auto eboScope = shaderMachine.useElementBufferObject();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }
    }

    void prepareTexture() {
        {
            glGenTextures(1, &texture1);

            glBindTexture(GL_TEXTURE_2D, texture1);

            // 前面提到的每个选项都可以使用glTexParameter*函数对单独的一个坐标轴设置（s、t（如果是使用3D纹理那么还有一个r）它们和x、y、z是等价的）
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // 缩小filter minify
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            // 放大filter magnify
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            std::vector<unsigned char> image;
            unsigned width;
            unsigned height;
            loadImage("container.png", image, width, height);
            // https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/06%20Textures/
            glTexImage2D(
                    // 第一个参数指定了纹理目标(Target)。
                    // 设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理
                    // 对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D
                    // 和GL_TEXTURE_3D的纹理不会受到影响）。
                    GL_TEXTURE_2D,
                    // 第二个参数为纹理指定多级渐远纹理的级别，
                    // 如果你希望单独手动设置每个多级渐远纹理的级别的话。
                    // 这里我们填0，也就是基本级别。
                    0,
                    // 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。
                    // 我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
                    GL_RGB,
                    // 第四个和第五个参数设置最终的纹理的宽度和高度。
                    // 我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
                    width, height,
                    // 下个参数应该总是被设为0（历史遗留问题）。
                    0,
                    // 第七第八个参数定义了源图的格式和数据类型。
                    // 我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，
                    // 我们将会传入对应值。
                    GL_RGB, GL_UNSIGNED_BYTE,
                    // 最后一个参数是真正的图像数据。
                    image.data()
            );

            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            image.clear();
            glCheckError();
        }

        {
            glGenTextures(1, &texture2);
            glBindTexture(GL_TEXTURE_2D, texture2);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            std::vector<unsigned char> image;
            unsigned width, height;
            loadImage("awesomeface.png", image, width, height);

            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RGB,
                    width, height,
                    0,
                    GL_RGB, GL_UNSIGNED_BYTE,
                    image.data());
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glCheckError();
        }
    }

    ~TransformationsRenderer() override {
        glDeleteTextures(1, &texture1);
    }

    void handleKeyEvent(int key, int action) override {
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            mixValue += 0.1f;
            mixValue = std::min(mixValue, 1.0f);
        } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            mixValue -= 0.1f;
            mixValue = std::max(mixValue, 0.0f);
        }
    }

    void render() override {
        if (!shaderMachine.success()) return;

        gl::Scope p(shaderMachine);

        glCheckError();
        // texture unit
        glActiveTexture(GL_TEXTURE0);
        // bind texture1 to texture-unit 0
        glBindTexture(GL_TEXTURE_2D, texture1);
        // bind texture2 to texture-unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUniform1i(shaderMachine.extra.texture1, 0);
        glUniform1i(shaderMachine.extra.texture2, 1);
        glUniform1f(shaderMachine.extra.mixValue, mixValue);

        auto transform = glm::identity<glm::mat4>();
        transform = glm::translate(transform, glm::vec3(0.5, -0.5, 0.0));
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count() % 10000;
        transform = glm::rotate(
                transform,
                 time * 0.002f,
                glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(shaderMachine.extra.transform, 1, GL_FALSE, glm::value_ptr(transform));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glCheckError();

        glBindVertexArray(0);
    }

};

Renderer *makeTransformationsRenderer() {
    return new TransformationsRenderer();
}

