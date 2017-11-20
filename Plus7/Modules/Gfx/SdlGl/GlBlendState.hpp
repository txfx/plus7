#pragma once

#include <Modules/Gfx/BlendProperties.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {
struct GlBlendState
{
public:
    explicit GlBlendState(const BlendProperties& _properties);

    void Bind() const;

private:
    const bool   enable;
    const GLenum srcRgb;
    const GLenum dstRgb;
    const GLenum opRgb;
    const GLenum srcAlpha;
    const GLenum dstAlpha;
    const GLenum opAlpha;
};
} // namespace gfx
} // namespace p7