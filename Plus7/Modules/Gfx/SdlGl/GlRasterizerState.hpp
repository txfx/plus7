#pragma once

#include <Modules/Gfx/RasterizerProperties.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {

struct GlRasterizerState
{
public:
    explicit GlRasterizerState(const RasterizerProperties& _properties);

    void Apply() const;

private:
    const GLenum fillmode;
    const GLenum cullmode;
    const GLenum frontface;
    const bool   cull;
};
} // namespace gfx
} // namespace p7