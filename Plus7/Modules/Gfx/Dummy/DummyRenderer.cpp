#include "DummyRenderer.hpp"

namespace p7 {
namespace gfx {

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
    return BufferSpan{ CreateBuffer(_properties, _data), 0, _properties.size };
}

} // namespace gfx
} // namespace p7