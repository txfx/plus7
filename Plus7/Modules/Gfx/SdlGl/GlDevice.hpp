#pragma once

#include <Modules/Gfx/DeviceInterface.hpp>

namespace p7 {
namespace gfx {

struct GlDevice : public DeviceInterface
{
public:
    // Texture
    TexturePtr CreateTexture(uint32_t _w, uint32_t _h, const void* _data) override;

    // Buffer
    BufferPtr  CreateBuffer(uint32_t _size, const void* _data) override;
    BufferSpan CreateTempBuffer(uint32_t _size, const void* _data) override;
};
} // namespace gfx
} // namespace p7