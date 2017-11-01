#pragma once

#include "BlendState.hpp"
#include "DepthState.hpp"
#include "RasterizerState.hpp"

namespace p7 {
namespace gfx {

struct Buffer;

struct RendererInterface
{
public:
    virtual void Clear(float _r, float _g, float _b, float _a) = 0;
    inline void  Clear(float _r, float _g, float _b) { Clear(_r, _g, _b, 1.0f); }

    virtual void SetViewport(float _x, float _y, float _w, float _h) = 0;
    virtual void SetScissor(float _x, float _y, float _w, float _h)  = 0;

    virtual void BindIndexBuffer(Buffer& _buffer)                      = 0;
    virtual void BindVertexBuffer(Buffer& _buffer, uint8_t _binding)   = 0;
    virtual void BindConstantBuffer(Buffer& _buffer, uint8_t _binding) = 0;

    virtual void BindBlendState(const BlendState& _state)           = 0;
    virtual void BindDepthState(const DepthState& _state)           = 0;
    virtual void BindRasterizerState(const RasterizerState& _state) = 0;
};

} // namespace gfx
} // namespace p7