#pragma once

#include <Utils/NonCopyable.hpp>

#include "Buffer.hpp"
#include "Texture.hpp"

namespace p7 {
namespace gfx {

struct DeviceInterface : public NonCopyable
{
public:
    virtual ~DeviceInterface() = default;

    // Texture
    virtual TexturePtr CreateTexture(uint32_t _w, uint32_t _h, const void* _data) = 0;

    // Buffer
    virtual BufferPtr  CreateBuffer(uint32_t _size, const void* _data)     = 0;
    virtual BufferSpan CreateTempBuffer(uint32_t _size, const void* _data) = 0;
};
} // namespace gfx
} // namespace p7