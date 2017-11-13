#pragma once

#include "BlendState.hpp"
#include "DepthState.hpp"
#include "RasterizerState.hpp"
#include "ShaderState.hpp"
#include "Texture.hpp"

#include "Buffer.hpp"

namespace p7 {
namespace gfx {

struct RendererInterface
{
public:
    // clear
    virtual void Clear(float _r, float _g, float _b, float _a) = 0;
    inline void  Clear(float _r, float _g, float _b) { Clear(_r, _g, _b, 1.0f); }

    // viewport & scissors
    virtual void SetViewport(float _x, float _y, float _w, float _h) = 0;
    virtual void SetScissor(float _x, float _y, float _w, float _h)  = 0;

    // Buffers
    virtual void BindIndexBuffer(const Buffer& _buffer, uint32_t _offset, uint32_t _size)                      = 0;
    virtual void BindVertexBuffer(const Buffer& _buffer, uint32_t _offset, uint32_t _size, uint8_t _binding)   = 0;
    virtual void BindConstantBuffer(const Buffer& _buffe, uint32_t _offset, uint32_t _sizer, uint8_t _binding) = 0;

    inline void BindIndexBuffer(const Buffer& _buffer) { BindIndexBuffer(_buffer, 0, _buffer.GetSize()); }
    inline void BindIndexBuffer(const BufferSpan& _span) { BindIndexBuffer(_span.GetBuffer(), _span.GetOffset(), _span.GetSize()); }

    inline void BindVertexBuffer(const Buffer& _buffer, uint8_t _binding) { BindVertexBuffer(_buffer, 0, _buffer.GetSize(), _binding); }
    inline void BindVertexBuffer(const BufferSpan& _span, uint8_t _binding) { BindVertexBuffer(_span.GetBuffer(), _span.GetOffset(), _span.GetSize(), _binding); }

    inline void BindConstantBuffer(const Buffer& _buffer, uint8_t _binding) { BindConstantBuffer(_buffer, 0, _buffer.GetSize(), _binding); }
    inline void BindConstantBuffer(const BufferSpan& _span, uint8_t _binding) { BindConstantBuffer(_span.GetBuffer(), _span.GetOffset(), _span.GetSize(), _binding); }

    // Render states
    virtual void BindBlendState(const BlendState& _state)           = 0;
    virtual void BindDepthState(const DepthState& _state)           = 0;
    virtual void BindRasterizerState(const RasterizerState& _state) = 0;

    virtual void BindShaderState(const ShaderState& _state) = 0;

    // Texture
    virtual void BindTexture(const TexturePtr& _texture, uint8_t _binding) = 0;

    // Draw
    virtual void Draw(uint32_t _vertexCount, uint32_t _instanceCount, uint32_t _firstVertex, uint32_t _firstInstance)                             = 0;
    virtual void DrawIndexed(uint32_t _indexCount, uint32_t _instanceCount, uint32_t _firstIndex, int32_t _vertexOffset, uint32_t _firstInstance) = 0;

    inline void Draw(uint32_t _vertexCount) { Draw(_vertexCount, 1, 0, 0); }
    inline void Draw(uint32_t _vertexCount, uint32_t _instanceCount) { Draw(_vertexCount, _instanceCount, 0, 0); }

    inline void DrawIndexed(uint32_t _indexCount) { DrawIndexed(_indexCount, 1, 0, 0, 0); }
    inline void DrawIndexed(uint32_t _indexCount, uint32_t _instanceCount) { DrawIndexed(_indexCount, _instanceCount, 0, 0, 0); }
};

} // namespace gfx
} // namespace p7