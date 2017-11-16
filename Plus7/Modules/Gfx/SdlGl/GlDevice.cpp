#include "GlDevice.hpp"

namespace p7 {
namespace gfx {

TexturePtr GlDevice::CreateTexture(uint32_t /* _w */, uint32_t /* _h */, const void* /* _data */)
{
    return std::make_shared<Texture>();
}

// Buffer
BufferPtr GlDevice::CreateBuffer(uint32_t _size, const void* _data)
{
    return std::make_shared<GlBuffer>(_size, _data);
}

BufferSpan GlDevice::CreateTempBuffer(uint32_t /* _size */, const void* /* _data */)
{
    return BufferSpan{};
}

} // namespace gfx
} // namespace p7