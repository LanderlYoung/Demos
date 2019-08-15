/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-15
* Time:   17:02
* Life with Passion, Code with Creativity.
* </pre>
*/
#include "AxisRender.h"

static constexpr auto vertexShader = R"(
#version 330 core

layout(location = 0) in vec3 point;
layout(location = 1) in vec3 color;

out vec3 LineColor;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(point, 1.0f);
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

static constexpr float FAR = 1000;

static GLfloat points[36]{
        // -x start | color
        -FAR, 0.0f, 0.0f,/**/ 1.0, 0.0f, 0.0f,
        // +x end   | color
        FAR, 0.0f, 0.0f, /**/ 1.0, 0.0f, 0.0f,
        // -y start | color
        0.0f, -FAR, 0.0f,/**/ 0.0f, 1.0, 0.0f,
        // +y end   | color
        0.0f, FAR, 0.0f, /**/ 0.0f, 1.0, 0.0f,
        // -z start | color,
        0.0f, 0.0f, -FAR,/**/ 0.0f, 0.0f, 1.0,
        // +z end   | color
        0.0f, 0.0f, FAR, /**/ 0.0f, 0.0f, 1.0
};

AxisRenderer::AxisRenderer() : shaderMachine(vertexShader, fragmentShader, [](auto &p, auto &u) {
    u.projection = p.getUniformLocation("projection");
    u.view = p.getUniformLocation("view");
}) {

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

    setTranslate(glm::identity<glm::mat4>(), glm::identity<glm::mat4>());
    glCheckError();
}

void AxisRenderer::render(float width, float height) {
    gl::Scope s(shaderMachine);
    glDrawArrays(GL_LINES, 0, 6);
}

void AxisRenderer::setTranslate(const glm::mat4 &projection, const glm::mat4 &view) {
    gl::Scope s(shaderMachine);

    glUniformMatrix4fv(shaderMachine.extra.projection, 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(shaderMachine.extra.view, 1, false, glm::value_ptr(view));
}

Renderer *makeAxisRenderer() {
    return new AxisRenderer();
}
