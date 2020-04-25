#include "Dummy/DummyRenderer.hpp"

#include "Dummy/DummyCommandBuffer.hpp"

namespace p7::gfx {

DummyCommandBuffer& DummyRenderer::GetCommandBuffer()
{
    static DummyCommandBuffer cb;
    return cb;
}

TexturePtr DummyRenderer::CreateTexture(const TextureProperties& _properties, const void* _data)
{
    return std::make_shared<Texture>(_properties, _data);
}

BufferPtr DummyRenderer::CreateBuffer(const BufferProperties& _properties, const void* _data)
{
    return std::make_shared<Buffer>(_properties, _data);
}

BufferSpan DummyRenderer::CreateTempBuffer(const BufferProperties& _properties, const void* _data)
{
    return BufferSpan { CreateBuffer(_properties, _data), 0, _properties.size };
}

} // namespace p7::gfx
