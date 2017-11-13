#pragma once

#include <Utils/NonCopyable.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {
struct GlShaderState : public NonCopyable
{
public:
    explicit GlShaderState(const char* _vertex, const char* _pixel);
    ~GlShaderState();

    void Apply() const;

private:
    const GLuint program;
};
} // namespace gfx
} // namespace p7