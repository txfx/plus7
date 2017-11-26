#pragma once

#include <cstdint>

#include <Modules/Gfx/Buffer.hpp>
#include <Modules/Gfx/Texture.hpp>
#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace gfx {

struct DummyRenderer
{
public:
    virtual ~DummyRenderer() = default;

    // Texture
    virtual TexturePtr CreateTexture(const TextureProperties& _properties, const void* _data);

    // Buffer
    virtual BufferPtr  CreateBuffer(const BufferProperties& _properties, const void* _data);
    virtual BufferSpan CreateTempBuffer(const BufferProperties& _properties, const void* _data);

    virtual uint32_t GetWidth() const { return 0; }
    virtual uint32_t GetHeight() const { return 0; }
};
} // namespace gfx
} // namespace p7