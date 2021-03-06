#pragma once

#include "DepthProperties.hpp"

#include "SdlGl/gl_headers.h"

namespace p7 {
namespace gfx {

struct GlDepthState
{
public:
    explicit GlDepthState(const DepthProperties& _properties);

    void Bind() const;

private:
    const bool   enable;
    const bool   write;
    const GLenum func;
    const bool   clip;
    const float  min;
    const float  max;
};

} // namespace gfx
} // namespace p7