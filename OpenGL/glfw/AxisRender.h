/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-15
* Time:   17:20
* Life with Passion, Code with Creativity.
* </pre>
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "index.h"
#include "glhelper.h"

class AxisRenderer : public Renderer {
private:
    struct Extra {
        GLint projection;
        GLint view;
    };
    gl::ShaderMachine<false, Extra> shaderMachine;
public:
    AxisRenderer();

    void render(float width, float height) override;

    void setTranslate(
            const glm::mat4 &projection,
            const glm::mat4 &view);
};
