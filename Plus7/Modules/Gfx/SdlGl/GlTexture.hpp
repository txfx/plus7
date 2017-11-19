#pragma once

#include <Modules/Gfx/Dummy/DummyTexture.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {

struct GlTexture final : public DummyTexture
{
    explicit GlTexture(const TextureProperties& _properties, const void* _data);
    ~GlTexture();

private:
    GLuint texture;
};

} // namespace gfx
} // namespace p7