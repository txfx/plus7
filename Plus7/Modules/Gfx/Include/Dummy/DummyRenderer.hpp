#pragma once

#include <cstdint>

#include "Buffer.hpp"
#include "Texture.hpp"

#include <Tasks/Task.hpp>
#include <Utils/NonCopyable.hpp>

namespace p7::gfx {

struct DummyCommandBuffer;

struct DummyRenderer
{
public:
    virtual ~DummyRenderer() = default;

    // Command buffer
    virtual DummyCommandBuffer& GetCommandBuffer();

    // Texture
    virtual TexturePtr CreateTexture(const TextureProperties& _properties, const void* _data);

    // Buffer
    virtual BufferPtr  CreateBuffer(const BufferProperties& _properties, const void* _data);
    virtual BufferSpan CreateTempBuffer(const BufferProperties& _properties, const void* _data);

    virtual uint32_t GetWidth() const { return 0; }
    virtual uint32_t GetHeight() const { return 0; }

    virtual tasks::TypedID<void> GetEndFrameTask() const = 0;
};
} // namespace p7::gfx
