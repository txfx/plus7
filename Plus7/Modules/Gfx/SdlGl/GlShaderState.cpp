#include "GlShaderState.hpp"

namespace p7 {
namespace gfx {
GlShaderState::GlShaderState(const char* _vertex, const char* _pixel)
    : program(glCreateProgram())
    , vertex(glCreateShader(GL_VERTEX_SHADER))
    , pixel(glCreateShader(GL_FRAGMENT_SHADER))
{
    glShaderSource(vertex, 1, &_vertex, nullptr);
    glShaderSource(pixel, 1, &_pixel, nullptr);
    glCompileShader(vertex);
    glCompileShader(pixel);
    glAttachShader(program, vertex);
    glAttachShader(program, pixel);
    glLinkProgram(program);
}

void GlShaderState::Apply() const
{
    glUseProgram(program);
}

} // namespace gfx
} // namespace p7