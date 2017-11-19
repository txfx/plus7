#pragma once

#include <Modules/Gfx/TextureProperties.hpp>

namespace p7 {
namespace gfx {

struct DummyTexture
{
    explicit DummyTexture(const TextureProperties& _properties)
        : properties(_properties)
    {}

    TextureProperties properties;
};

} // namespace gfx
} // namespace p7