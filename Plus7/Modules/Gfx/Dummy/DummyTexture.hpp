#pragma once

#include <Modules/Gfx/TextureProperties.hpp>
#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace gfx {

struct DummyTexture : public NonCopyable
{
    explicit DummyTexture(const TextureProperties& _properties, const void* /* _data */)
        : properties(_properties)
    {}

    TextureProperties properties;
};

} // namespace gfx
} // namespace p7