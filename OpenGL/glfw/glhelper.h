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
    explicit Shader(const std::string_view &shaderSource);

    DISALLOW_COPY_ASSIGN(Shader);

    ~Shader() { if (program) glDeleteShader(program); }

    Shader(Shader &&from) noexcept : program(from.program), compileLog(from.compileLog) {
        from.program = 0;
        from.compileLog = "(deleted)";
    }

    static Shader<VertexOrFragment> &&fromFile(const std::string &filePath) {
        return std::move(Shader<VertexOrFragment>(readFile(filePath)));
    }

    bool success() const { return program != 0; }

    GLuint get() const { return program; }

    GLuint operator*() const { return program; }

    const std::string &getCompileLog() const { return compileLog; }
};

class Scope {
public:
    class Usable {
    public:
        virtual void use() = 0;

        virtual void unuse() = 0;
    };

private:
    static thread_local std::vector<Scope::Usable *> _usableStack;
    Usable *usable;

public:

    DISALLOW_COPY_ASSIGN(Scope);

    DISALLOW_DYNAMIC_CONSTRUCT(Scope);

    explicit Scope(Usable *usable) noexcept;

    explicit Scope(Usable &usable) noexcept: Scope(&usable) {}

    Scope(Scope &&from) noexcept : usable(from.usable) { from.usable = nullptr; }

    ~Scope();
};

class ShaderProgram : public Scope::Usable {

public:

private:
    GLuint program;
    std::string compileLog;

    void compileProgram(const std::string_view &vertexShader,
                        const std::string_view &fragmentShader);
public:
    ShaderProgram(const std::string_view &vertexShader,
                  const std::string_view &fragmentShader)
            : program(0), compileLog() {
        compileProgram(vertexShader, fragmentShader);
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

    GLuint getUniformLocation(const std::string_view &uniformName) const {
        return glGetUniformLocation(program, uniformName.data());
    }

    void use() override { glUseProgram(program); }

    void unuse() override { glUseProgram(0); }

};

template<typename Extra>
class ShaderProgramExtra : public ShaderProgram {
public:
    Extra location;

    ShaderProgramExtra(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader) :
            ShaderProgram(vertexShader, fragmentShader),
            location() {}

    ShaderProgramExtra(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader,
            const std::function<void(const ShaderProgram &, Extra &)> &uniformInit) :
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



