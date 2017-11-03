#pragma once

#include <Utils/NonCopyable.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {
struct GlShaderState : public NonCopyable
{
public:
    explicit GlShaderState(const char* _vertex, const char* _pixel);

    void Apply() const;

private:
    const GLuint program;
    const GLuint vertex;
    const GLuint pixel;
};
} // namespace gfx
} // namespace p7