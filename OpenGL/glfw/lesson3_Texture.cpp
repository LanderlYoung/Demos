/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-12
* Time:   14:15
* Life with Passion, Code with Creativity.
* </pre>
*/
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

void main() {
    gl_Position = vec4(position, 1.0f);
    ourColor = color;
    TexCoord = texCoord;
}
)";

constexpr auto fragmentShader = R"(
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = texture(ourTexture, TexCoord);
}

)";

class TextureRenderer : public Renderer {
private:
    gl::ShaderProgram shaderProgram;
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint texture = 0;
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

    static unsigned loadImage(std::vector<unsigned char> &image, unsigned &width, unsigned &height) {
        std::string base = __FILE__;
        auto file = base.substr(0, base.rfind('/') + 1) + "assets/container.png";
        return lodepng::decode(image, width, height, file, LodePNGColorType::LCT_RGB);
    }

public:
    TextureRenderer() : shaderProgram(vertexShader, fragmentShader) {
        if (!shaderProgram.success()) {
            std::cerr << "compile shader failed " << shaderProgram.getCompileLog() << std::endl;
            return;
        }

        prepareVertexData();

        prepareTexture();
    }

    void prepareVertexData() {
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
                    8 * sizeof(GLfloat),
                    // offset?
                    nullptr
            );
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
                    8 * sizeof(GLfloat),
                    // offset?
                    (void *) (3 * sizeof(GLfloat))
            );
            glVertexAttribPointer(
                    // location in shader
                    2,
                    // size
                    2,
                    // type
                    GL_FLOAT,
                    // normalize to [-1, 1]
                    GL_FALSE,
                    // stride (length of each element)
                    8 * sizeof(GLfloat),
                    // offset?
                    (void *) (6 * sizeof(GLfloat))
            );
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            {
                // create index for VBO
                glGenBuffers(1, &EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            // release bound VAO
            glBindVertexArray(0);
        }
    }

    void prepareTexture() {
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

        std::vector<unsigned char> image;
        unsigned width;
        unsigned height;
        loadImage(image, width, height);
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
        image.clear();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~TextureRenderer() override {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &texture);
    }

    void render() override {
        glBindTexture(GL_TEXTURE_2D, texture);
        auto scope = shaderProgram.use();
        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

};


Renderer *makeTextureRenderer() {
    return new TextureRenderer();
}

