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

class Renderer {
public:
    Renderer() = default;

    virtual ~Renderer() = default;

    virtual void render() = 0;

    virtual void handleKeyEvent(int key, int action) {
        // empty
    }
};

Renderer *makeTriangleRenderer();

Renderer *makeRectangleRenderer();

Renderer *makeShaderRenderer();

Renderer *makeTextureRenderer();

Renderer *makeTransformationsRenderer();

//constexpr auto makeRenderer = makeTriangleRenderer;
//constexpr auto makeRenderer = makeRectangleRenderer;
//constexpr auto makeRenderer = makeShaderRenderer;
//constexpr auto makeRenderer = makeTextureRenderer;
constexpr auto makeRenderer = makeTransformationsRenderer;
