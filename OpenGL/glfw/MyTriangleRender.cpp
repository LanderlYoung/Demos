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

template<bool VertexOrFragment>
class Shader {
private:
    GLuint program = 0;
    std::string compileLog;
public:
    explicit Shader(const std::string_view &shaderSource) : compileLog() {
        GLuint shaderId = 0;
        if (VertexOrFragment) {
            shaderId = glCreateShader(GL_VERTEX_SHADER);
        } else {
            shaderId = glCreateShader(GL_FRAGMENT_SHADER);
        }
        const GLchar *vs = shaderSource.data();
        const GLint vsl = shaderSource.length();
        glShaderSource(shaderId, 1, &vs, &vsl);
        glCompileShader(shaderId);

        GLint status;
        // params returns GL_TRUE if the last compile operation on
        // shader was successful, and GL_FALSE otherwise.
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
        if (status == GL_TRUE) {
            program = shaderId;
        } else {
            // params returns the number of characters in the information log
            // for shader including the null termination character
            // (i.e., the size of the character buffer required to
            // store the information log).
            // If shader has no information log, a value of 0 is returned.
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &status);
            compileLog.resize(status);
            glGetShaderInfoLog(shaderId, status, nullptr, compileLog.data());
            glDeleteShader(shaderId);
        }
    }

    DISALLOW_COPY_ASSIGN(Shader);

    ~Shader() {
        if (program) {
            glDeleteShader(program);
        }
    }

    Shader(Shader &&from) noexcept : program(from.program), compileLog(from.compileLog) {
        from.program = 0;
        from.compileLog = "(deleted)";
    }

    bool success() const {
        return program != 0;
    }

    GLuint get() const {
        return program;
    }

    GLuint operator*() const {
        return program;
    }

    [[nodiscard]] const std::string &getCompileLog() const {
        return compileLog;
    }
};

class ShaderProgram {
private:
    GLuint program = 0;
    std::string compileLog;
public:
    ShaderProgram(const std::string_view &vertexShader,
                  const std::string_view &fragmentShader) : compileLog() {
        Shader<true> vertex(vertexShader);
        if (vertex.success()) {
            Shader<false> fragment(fragmentShader);
            if (fragment.success()) {
                GLuint shaderProgram = glCreateProgram();
                glAttachShader(shaderProgram, *vertex);
                glAttachShader(shaderProgram, *fragment);

                glLinkProgram(shaderProgram);

                GLint status;
                glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
                if (status == GL_TRUE) {
                    program = shaderProgram;
                } else {
                    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &status);
                    compileLog.resize(status);

                    glGetShaderInfoLog(shaderProgram, compileLog.length(), nullptr, compileLog.data());
                    glDeleteShader(shaderProgram);
                }
            } else {
                compileLog = "[fragmentShader]" + fragment.getCompileLog();
            }
        } else {
            compileLog = "[vertexShader]" + vertex.getCompileLog();
        }
    }

    DISALLOW_COPY_ASSIGN(ShaderProgram);

    ~ShaderProgram() {
        if (program) {
            glDeleteProgram(program);
        }
    }

    ShaderProgram(ShaderProgram &&from) noexcept : program(from.program), compileLog(from.compileLog) {
        from.program = 0;
        from.compileLog = "(deleted)";
    }

    bool success() const {
        return program != 0;
    }

    GLuint get() const {
        return program;
    }

    GLuint operator*() const {
        return program;
    }

    void use() {
        if (success()) {
            glUseProgram(program);
        }
    }

    const std::string &getCompileLog() const {
        return compileLog;
    }
};

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
    ShaderProgram shaderProgram;
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
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // release bound array
        glBindVertexArray(0);
    }

};

Renderer *makeMyTriangleRenderer() {
    return new MyTriangleRenderer();
}
