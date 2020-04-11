#include "SdlGl/GlShaderState.hpp"

namespace p7 {
namespace gfx {
GlShaderState::GlShaderState(const char* _vertex, const char* _pixel)
    : program(glCreateProgram())
{
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint pixel  = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &_vertex, nullptr);
    glShaderSource(pixel, 1, &_pixel, nullptr);
    glCompileShader(vertex);
    glCompileShader(pixel);
    glAttachShader(program, vertex);
    glAttachShader(program, pixel);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(pixel);
}

GlShaderState::~GlShaderState()
{
    glDeleteProgram(program);
}

void GlShaderState::Bind() const
{
    glUseProgram(program);
}

} // namespace gfx
} // namespace p7