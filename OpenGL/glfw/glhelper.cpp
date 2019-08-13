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

std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

GLuint ShaderProgram::compileProgram(
        const std::string_view &vertexShader,
        const std::string_view &fragmentShader,
        std::string &compileLog) {
    GLuint program = 0;
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
    return program;
}

thread_local std::vector<GLuint> ShaderProgram::Scope::_programScopeStack;

ShaderProgram::Scope::Scope(GLuint programId) noexcept
        : programId(programId) {
    if (programId) {
        _programScopeStack.push_back(programId);
        glUseProgram(programId);
    }
}

ShaderProgram::Scope::~Scope() {
    if (programId) {
        // assert legit stack status
        assert(!_programScopeStack.empty());
        assert(_programScopeStack.at(_programScopeStack.size() - 1) == programId);

        // pop current scope
        _programScopeStack.pop_back();

        // restore previous scope
        if (_programScopeStack.empty()) {
            glUseProgram(0);
        } else {
            glUseProgram(_programScopeStack.at(_programScopeStack.size() - 1));
        }
    }
}

}
