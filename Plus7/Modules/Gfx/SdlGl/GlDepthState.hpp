#pragma once

#include <Modules/Gfx/DepthProperties.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {

struct GlDepthState
{
public:
    GlDepthState(const DepthProperties& _properties);

    void Apply() const;

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