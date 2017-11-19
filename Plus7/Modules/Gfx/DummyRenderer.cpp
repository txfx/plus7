#include "DummyRenderer.hpp"

namespace p7 {
namespace gfx {

TexturePtr DummyRenderer::CreateTexture(uint32_t /* _w */, uint32_t /* _h */, const void* /* _data */)
{
    return std::make_shared<Texture>();
}

// Buffer
BufferPtr DummyRenderer::CreateBuffer(uint32_t _size, const void* _data)
{
    return std::make_shared<GlBuffer>(_size, _data);
}

BufferSpan DummyRenderer::CreateTempBuffer(uint32_t /* _size */, const void* /* _data */)
{
    return BufferSpan{};
}

} // namespace gfx
} // namespace p7