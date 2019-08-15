/*
 * <pre>
 * Author: taylorcyang@tencent.com
 * Date:   2019-08-05
 * Time:   11:40
 * Life with Passion, Code with Creativity.
 * </pre>
 */

#pragma once

// include gl header
#define GLEW_STATIC
#include <GL/glew.h>
// glfw
#include <GLFW/glfw3.h>

#include "utils.h"
#include "glhelper.h"

class Renderer {
public:
    Renderer() = default;

    virtual ~Renderer() = default;

    virtual void render(float width, float height) = 0;

    virtual void handleKeyEvent(int key, int action) {
        // empty
    }
};

class AxisRenderer : public Renderer{
private:
    gl::ShaderMachine<false> shaderMachine;
public:
    AxisRenderer();
    void render(float width, float height) override;
};

Renderer *makeAxisRenderer();

Renderer *makeTriangleRenderer();

Renderer *makeRectangleRenderer();

Renderer *makeShaderRenderer();

Renderer *makeTextureRenderer();

Renderer *makeTransformationsRenderer();

Renderer *makeCoordinateSystemRenderer();

//constexpr auto makeRenderer = makeAxisRenderer;
//constexpr auto makeRenderer = makeTriangleRenderer;
//constexpr auto makeRenderer = makeRectangleRenderer;
//constexpr auto makeRenderer = makeShaderRenderer;
//constexpr auto makeRenderer = makeTextureRenderer;
constexpr auto makeRenderer = makeCoordinateSystemRenderer;
