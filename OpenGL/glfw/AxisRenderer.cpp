/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-15
* Time:   17:02
* Life with Passion, Code with Creativity.
* </pre>
*/
#include "index.h"

static constexpr auto vertexShader = R"(
#version 330 core

layout(location = 0) in vec3 point;
layout(location = 1) in vec3 color;

out vec3 LineColor;

void main() {
    gl_Position = vec4(point, 1.0f);
    LineColor = color;
}

)";
static constexpr auto fragmentShader = R"(
#version 330 core

out vec4 color;
in vec3 LineColor;

void main() {
    color = vec4(LineColor, 1.0f);
}
)";

static GLfloat points[36]{
        // -x start | color
        -1.0f, 0.0f, 0.0f,/**/ 1.0f, 0.0f, 0.0f,
        // +x end   | color
        1.0f, 0.0f, 0.0f, /**/ 1.0f, 0.0f, 0.0f,
        // -y start | color
        0.0f, -1.0f, 0.0f,/**/ 0.0f, 1.0f, 0.0f,
        // +y end   | color
        0.0f, 1.0f, 0.0f, /**/ 0.0f, 1.0f, 0.0f,
        // -z start | color,
        0.0f, 0.0f, -1.0f,/**/ 0.0f, 0.0f, 1.0f,
        // +z end   | color
        0.0f, 0.0f, 1.0f, /**/ 0.0f, 0.0f, 1.0f
};

AxisRenderer::AxisRenderer() : shaderMachine(vertexShader, fragmentShader) {
    if (!shaderMachine.success()) {
        std::cerr << shaderMachine.getCompileLog() << std::endl;
    }
    gl::Scope s(shaderMachine);
    auto vaoS = shaderMachine.useVertexArrayObject();
    auto vboS = shaderMachine.useVertexBufferObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glCheckError();
}

void AxisRenderer::render(float width, float height) {
    gl::Scope s(shaderMachine);
    glDrawArrays(GL_LINES, 0, 6);
}

Renderer *makeAxisRenderer() {
    return new AxisRenderer();
}
