/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-05
* Time:   15:30
* Life with Passion, Code with Creativity.
* </pre>
*/

#include <fstream>
#include "glhelper.h"

namespace gl {

static void compileShader(
        bool vertexOrFragment,
        const std::string_view &shaderSource,
        GLuint &program,
        std::string &compileLog) {
    GLuint shaderId = 0;
    if (vertexOrFragment) {
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

// 特化
template<>
Shader<true>::Shader(const std::string_view &shaderSource) : compileLog() {
    compileShader(true, shaderSource, program, compileLog);
}

// 特化
template<>
Shader<false>::Shader(const std::string_view &shaderSource) : compileLog() {
    compileShader(false, shaderSource, program, compileLog);
}

std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void ShaderProgram::compileProgram(
        const std::string_view &vertexShader,
        const std::string_view &fragmentShader) {
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

thread_local std::vector<Scope::Usable *> Scope::_usableStack;

Scope::Scope(Usable *usable) noexcept : usable(usable) {
    assert(usable != nullptr);
    _usableStack.push_back(usable);
    usable->use();
}

Scope::~Scope() {
    if (usable) {
        // assert legit stack status
        assert(!_usableStack.empty());
        assert(_usableStack.at(_usableStack.size() - 1) == usable);

        // pop current scope
        _usableStack.pop_back();
        usable->unuse();

        // restore previous scope
        if (!_usableStack.empty()) {
            auto previous = _usableStack.at(_usableStack.size() - 1);
            previous->use();
        }
    }
}


}
