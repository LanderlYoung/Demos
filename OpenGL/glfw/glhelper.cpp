/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-05
* Time:   15:30
* Life with Passion, Code with Creativity.
* </pre>
*/

#include "glhelper.h"

namespace gl {
thread_local std::vector<GLuint> ShaderProgram::Scope::_programScopeStack(128);

}
