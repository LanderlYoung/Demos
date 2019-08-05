/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-05
* Time:   15:30
* Life with Passion, Code with Creativity.
* </pre>
*/

#pragma once


#include <string>
#include <string_view>
#include <vector>
#include <GL/glew.h>

#include "utils.h"

namespace gl {

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

public:
    class Scope {
    private:
        static thread_local std::vector<GLuint> _programScopeStack;
        GLuint programId;

    public:
        DISALLOW_COPY_ASSIGN(Scope);

        Scope(Scope &&from) : programId(from.programId) {
            from.programId = 0;
        }

        explicit Scope(const ShaderProgram &program) noexcept
                : programId(program.program) {
            if (program.program) {
                _programScopeStack.push_back(program.program);
                glUseProgram(program.program);
            }
        }

        ~Scope() {
            if (programId) {
                assert(!_programScopeStack.empty());
                assert(_programScopeStack.at(_programScopeStack.size() - 1) == programId);
                _programScopeStack.pop_back();

                if (_programScopeStack.empty()) {
                    glUseProgram(0);
                } else {
                    glUseProgram(_programScopeStack.at(_programScopeStack.size() - 1));
                }
            }
        }
    };

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

    Scope use() {
        return std::move(Scope(*this));
    }

    const std::string &getCompileLog() const {
        return compileLog;
    }

};

}



