#include "GlTexture.hpp"

namespace p7 {
namespace gfx {

namespace {
GLenum ToGl(PixelFormat _format)
{
    switch (_format)
    {
    case PixelFormat::R8G8B8A8_UNORM:
        return GL_RGBA8;
    }
}

} // namespace

GlTexture::GlTexture(const TextureProperties& _properties, const void* _data)
    : DummyTexture(_properties, _data)
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, ToGl(properties.format), properties.width, properties.height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, properties.width, properties.height, GL_RGBA, GL_UNSIGNED_BYTE, _data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GlTexture::~GlTexture()
{
    glDeleteTextures(1, &texture);
}
} // namespace gfx
} // namespace p7