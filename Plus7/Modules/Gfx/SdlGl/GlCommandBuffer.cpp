#include "GlCommandBuffer.hpp"

namespace p7 {
namespace gfx {

void GlCommandBuffer::Clear(float _r, float _g, float _b, float _a)
{
    glClearColor(_r, _g, _b, _a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GlCommandBuffer::SetViewport(float /* _x */, float /* _y */, float /* _w */, float /* _h */) {}
void GlCommandBuffer::SetScissor(float /* _x */, float /* _y */, float /* _w */, float /* _h */) {}
void GlCommandBuffer::BindIndexBuffer(const BufferPtr& /* _buffer */, uint32_t /* _offset */, uint32_t /* _size */) {}
void GlCommandBuffer::BindVertexBuffer(const BufferPtr& /* _buffer */, uint32_t /* _offset */, uint32_t /* _size */, uint8_t /* _binding */) {}
void GlCommandBuffer::BindConstantBuffer(const BufferPtr& /* _buffer */, uint32_t /* _offset */, uint32_t /* _size */, uint8_t /* _binding */) {}

void GlCommandBuffer::BindBlendState(const BlendState& _state)
{
    _state.Bind();
}

void GlCommandBuffer::BindDepthState(const DepthState& _state)
{
    _state.Bind();
}

void GlCommandBuffer::BindRasterizerState(const RasterizerState& _state)
{
    _state.Bind();
}

void GlCommandBuffer::BindShaderState(const ShaderState& _state)
{
    _state.Bind();
}

void GlCommandBuffer::BindTexture(const TexturePtr& /* _texture */, uint8_t /* _binding */) {}

void GlCommandBuffer::Draw(
    uint32_t _vertexCount,
    uint32_t _instanceCount,
    uint32_t _firstVertex,
    uint32_t _firstInstance)
{
    glDrawArraysInstancedBaseInstance(
        GL_TRIANGLES,
        _firstVertex,
        _vertexCount,
        _instanceCount,
        _firstInstance);
}

void GlCommandBuffer::DrawIndexed(
    uint32_t _indexCount,
    uint32_t _instanceCount,
    uint32_t _firstIndex,
    int32_t  _vertexOffset,
    uint32_t _firstInstance)
{
    glDrawElementsInstancedBaseVertexBaseInstance(
        GL_TRIANGLES,
        _indexCount,
        GL_UNSIGNED_SHORT,
        (void*)(uintptr_t)(_firstIndex * 2),
        _instanceCount,
        _vertexOffset,
        _firstInstance);
}

} // namespace gfx
} // namespace p7