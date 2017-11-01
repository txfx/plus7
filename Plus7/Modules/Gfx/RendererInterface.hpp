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

    virtual void Draw(uint32_t _vertexCount, uint32_t _instanceCount, uint32_t _firstVertex, uint32_t _firstInstance)                             = 0;
    virtual void DrawIndexed(uint32_t _indexCount, uint32_t _instanceCount, uint32_t _firstIndex, int32_t _vertexOffset, uint32_t _firstInstance) = 0;

    void Draw(uint32_t _vertexCount) { Draw(_vertexCount, 1, 0, 0); }
    void Draw(uint32_t _vertexCount, uint32_t _instanceCount) { Draw(_vertexCount, _instanceCount, 0, 0); }

    void DrawIndexed(uint32_t _indexCount) { DrawIndexed(_indexCount, 1, 0, 0, 0); }
    void DrawIndexed(uint32_t _indexCount, uint32_t _instanceCount) { DrawIndexed(_indexCount, _instanceCount, 0, 0, 0); }
};

} // namespace gfx
} // namespace p7