#pragma once

#include <cstdint>

namespace p7 {
namespace gfx {

enum class BufferType
{
    Vertex,
    Index,
    Constant
};

struct BufferProperties
{
    BufferType type;
    uint32_t   size;
};

} // namespace gfx
} // namespace p7