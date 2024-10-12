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

#ifndef NDEBUG

#include <thread>

#endif

#include <GL/glew.h>
#include <iostream>

#include "utils.h"

namespace gl {

std::string readFile(const std::string_view &filePath);

// use GL_LOG
GLenum __glCheckError(const char *func, int line);

#define glCheckError() gl::__glCheckError(__func__, __LINE__)

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
    protected:
        virtual void use() = 0;

        virtual void unuse() = 0;

        friend class Scope;
    };

private:
    static thread_local std::vector<Scope::Usable *> _usableStack;
    Usable *usable;
#ifndef NDEBUG
    // check for scope being used on wrong thread
    std::thread::id _thread_id;
#endif

    explicit Scope(Usable *usable) noexcept;

public:

    DISALLOW_COPY_ASSIGN(Scope);

    /**
     * stack allocation only
     */
    DISALLOW_DYNAMIC_CONSTRUCT(Scope);

    explicit Scope(Usable &usable) noexcept: Scope(&usable) {}

    Scope(Scope &&from) noexcept;

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

protected:
    void use() override { glUseProgram(program); }

    void unuse() override { glUseProgram(0); }

};

struct NoExtra {
};

template<typename Extra = NoExtra>
class ShaderProgramExtra : public ShaderProgram {
public:
    Extra extra;

    ShaderProgramExtra(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader) :
            ShaderProgram(vertexShader, fragmentShader),
            extra() {}

    ShaderProgramExtra(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader,
            const std::function<void(const ShaderProgram &, Extra &)> &uniformInit) :
            ShaderProgram(vertexShader, fragmentShader),
            extra() {
        if (success()) {
            uniformInit(static_cast<ShaderProgram &>(*this), extra);
        }
    }

    DISALLOW_COPY_ASSIGN(ShaderProgramExtra);

    ~ShaderProgramExtra() = default;

    ShaderProgramExtra(ShaderProgramExtra &&move) :
            ShaderProgram(move) {
        extra = std::move(move.extra);
    }
};


/**
 * A Shader Wrapper together with VertexArrayObject, VertexBufferObject, ElementBufferObject(optional).
 * @tparam useIndex  enable or not ElementBufferObject
 * @tparam Extra     extra data type (usually used to store uniform location in shader).
 */
template<bool useIndex = true, typename Extra = NoExtra>
class ShaderMachine : public ShaderProgramExtra<Extra> {

private:
    GLuint vertexArrayObject = 0;
    GLuint vertexBufferObject = 0;
    GLuint elementBufferObject = 0;

    template<int UsableType>
    class __Usable : public Scope::Usable {
        GLuint &i;
    public:

        __Usable(GLuint &i) : i(i) {
        }

    protected:
        void use() override {
            switch (UsableType) {
                case 1:
                    glBindVertexArray(i);
                    break;
                case 2:
                    glBindBuffer(GL_ARRAY_BUFFER, i);
                    break;
                case 3:
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
                    break;
            }
        }

        void unuse() override {
            switch (UsableType) {
                case 1:
                    glBindVertexArray(0);
                    break;
                case 2:
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    break;
                case 3:
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                    break;
            }
        }
    };

    __Usable<1> vertexArrayObjectUsable;
    __Usable<2> vertexBufferObjectUsable;
    __Usable<3> elementBufferObjectUsable;

    void initObjects() {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBufferObject);
        if (useIndex) {
            glGenBuffers(1, &elementBufferObject);
        }
    }

public:

    ShaderMachine(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader) :
            ShaderProgramExtra<Extra>(vertexShader, fragmentShader),
            vertexArrayObjectUsable(vertexArrayObject),
            vertexBufferObjectUsable(vertexBufferObject),
            elementBufferObjectUsable(elementBufferObject) {
        initObjects();
    }

    ShaderMachine(
            const std::string_view &vertexShader,
            const std::string_view &fragmentShader,
            const std::function<void(const ShaderProgram &, Extra &)> &uniformInit) :
            ShaderProgramExtra<Extra>(vertexShader, fragmentShader, uniformInit),
            vertexArrayObjectUsable(vertexArrayObject),
            vertexBufferObjectUsable(vertexBufferObject),
            elementBufferObjectUsable(elementBufferObject) {
        initObjects();
    }

    ShaderMachine(ShaderMachine &&move) noexcept :
            ShaderProgramExtra<Extra>(move),
            vertexArrayObject(move.vertexArrayObject),
            vertexBufferObject(move.vertexBufferObject),
            elementBufferObject(move.elementBufferObject),
            vertexArrayObjectUsable(vertexArrayObject),
            vertexBufferObjectUsable(vertexBufferObject),
            elementBufferObjectUsable(elementBufferObject) {
        move.vertexArrayObject = 0;
        move.vertexBufferObject = 0;
        move.elementBufferObject = 0;
    }

    ~ShaderMachine() {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &vertexBufferObject);
        glDeleteBuffers(1, &elementBufferObject);
    }

    DISALLOW_COPY_ASSIGN(ShaderMachine);

    Scope useVertexArrayObject() { return Scope(vertexArrayObjectUsable); }

    Scope useVertexBufferObject() { return Scope(vertexBufferObjectUsable); }

    Scope useElementBufferObject() { return Scope(elementBufferObjectUsable); }

protected:
    void use() override {
        ShaderProgramExtra<Extra>::use();
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        if (useIndex) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    }

    void unuse() override {
        ShaderProgramExtra<Extra>::unuse();
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (useIndex) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

}



