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

std::string readFile(const std::string_view &filePath);

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

    static Shader<VertexOrFragment> &&fromFile(const std::string &filePath) {
        return std::move(Shader<VertexOrFragment>(readFile(filePath)));
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

        explicit Scope(const ShaderProgram &program) noexcept : Scope(program.program) {}

        explicit Scope(GLuint programId) noexcept;

        Scope(Scope &&from) noexcept : programId(from.programId) { from.programId = 0; }

        ~Scope();

    };

private:
    GLuint program;
    std::string compileLog;

    static GLuint compileProgram(const std::string_view &vertexShader,
                                 const std::string_view &fragmentShader,
                                 std::string &compileLog);
public:
    ShaderProgram(const std::string_view &vertexShader,
                  const std::string_view &fragmentShader)
            : program(0), compileLog() {
        program = compileProgram(vertexShader, fragmentShader, compileLog);
    }

    DISALLOW_COPY_ASSIGN(ShaderProgram);

    ~ShaderProgram() {
        if (success()) {
            glDeleteProgram(program);
        }
    }

    ShaderProgram(ShaderProgram &&from) noexcept : program(from.program), compileLog(from.compileLog) {
        from.program = 0;
        from.compileLog = "(deleted)";
    }

    ShaderProgram &&fromFile(
            const std::string &vertexShaderPath,
            const std::string &fragmentShaderPath) {
        return std::move(ShaderProgram(readFile(vertexShaderPath), readFile(fragmentShaderPath)));
    }

    bool success() const { return program != 0; }

    GLuint get() const { return program; }

    GLuint operator*() const { return program; }

    const std::string &getCompileLog() const { return compileLog; }

    Scope use() const { return Scope(*this); }

    GLuint getUniformLocation(const std::string_view &uniformName) const {
        return glGetUniformLocation(program, uniformName.data());
    }

};

template<typename ShaderLocationStore>
class ShaderProgramExtra : public ShaderProgram {
public:
    ShaderLocationStore location;

    ShaderProgramExtra(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader) :
            ShaderProgram(vertexShader, fragmentShader),
            location() {}

    ShaderProgramExtra(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader,
            const std::function<void(const ShaderProgram &, ShaderLocationStore &)> &uniformInit) :
            ShaderProgram(vertexShader, fragmentShader),
            location() {
        if (success()) {
            uniformInit(static_cast<ShaderProgram&>(*this), location);
        }
    }

    DISALLOW_COPY_ASSIGN(ShaderProgramExtra);

    ~ShaderProgramExtra() = default;

    ShaderProgramExtra(ShaderProgramExtra &&move) :
            ShaderProgram(move) {
        location = std::move(move.location);
    }
};

}



