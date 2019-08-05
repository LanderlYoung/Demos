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

class Renderer {
public:
    Renderer() = default;

    virtual ~Renderer() = default;

    virtual void render() = 0;
};

Renderer *makeTriangleRenderer();