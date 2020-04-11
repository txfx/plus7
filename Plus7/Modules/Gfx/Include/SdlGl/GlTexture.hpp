#pragma once

#include "Dummy/DummyTexture.hpp"

#include "SdlGl/gl_headers.h"

namespace p7 {
namespace gfx {

struct GlTexture final : public DummyTexture
{
    explicit GlTexture(const TextureProperties& _properties, const void* _data);
    ~GlTexture();

    void Bind() const;

private:
    GLuint texture;
};

} // namespace gfx
} // namespace p7