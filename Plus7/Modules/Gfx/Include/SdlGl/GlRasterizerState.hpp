#pragma once

#include "RasterizerProperties.hpp"

#include "SdlGl/gl_headers.h"

namespace p7 {
namespace gfx {

struct GlRasterizerState
{
public:
    explicit GlRasterizerState(const RasterizerProperties& _properties);

    void Bind() const;

private:
    const GLenum fillmode;
    const GLenum cullmode;
    const GLenum frontface;
    const bool   cull;
    const bool   scissor;
};
} // namespace gfx
} // namespace p7