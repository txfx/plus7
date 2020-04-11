#pragma once

#include <cstdint>

namespace p7 {
namespace gfx {

enum class PixelFormat
{
    R8G8B8A8_UNORM
};

struct TextureProperties
{
    TextureProperties(uint32_t _w, uint32_t _h)
        : width(_w)
        , height(_h)
    {}

    PixelFormat format = PixelFormat::R8G8B8A8_UNORM;

    uint32_t width  = 1;
    uint32_t height = 1;
    uint32_t depth  = 1;
};
} // namespace gfx
} // namespace p7