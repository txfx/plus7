#pragma once

#include "BlendState.hpp"
#include "DepthState.hpp"
#include "RasterizerState.hpp"

namespace p7 {
namespace gfx {

struct RendererInterface
{
public:
    virtual void Clear(float _r, float _g, float _b, float _a) = 0;
    inline void  Clear(float _r, float _g, float _b) { Clear(_r, _g, _b, 1.0f); }

    virtual void SetBlendState(const BlendState& _state)           = 0;
    virtual void SetDepthState(const DepthState& _state)           = 0;
    virtual void SetRasterizerState(const RasterizerState& _state) = 0;
};

} // namespace gfx
} // namespace p7