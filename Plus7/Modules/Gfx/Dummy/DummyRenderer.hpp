#pragma once

#include <Utils/NonCopyable.hpp>

#include <Modules/Gfx/Buffer.hpp>
#include <Modules/Gfx/Texture.hpp>

namespace p7 {
namespace gfx {

struct DummyRenderer
{
public:
    virtual ~DummyRenderer() = default;

    // Texture
    virtual TexturePtr CreateTexture(const TextureProperties& _properties, const void* _data);

    // Buffer
    virtual BufferPtr  CreateBuffer(uint32_t _size, const void* _data);
    virtual BufferSpan CreateTempBuffer(uint32_t _size, const void* _data);
};
} // namespace gfx
} // namespace p7