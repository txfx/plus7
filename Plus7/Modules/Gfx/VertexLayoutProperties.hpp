#pragma once

#include <cstdint>
#include <initializer_list>

namespace p7 {
namespace gfx {

struct VertexAttribute
{
    enum class Type : uint8_t
    {
        R8G8B8A8_UNORM,
        XYZW32F
    };

    constexpr VertexAttribute(Type _type, uint8_t _binding)
        : type(_type)
        , binding(_binding)
        , offset(-1)
    {}

    constexpr VertexAttribute(Type _type, uint8_t _binding, uint16_t _offset)
        : type(_type)
        , binding(_binding)
        , offset(_offset)
    {}

    Type     type;
    uint8_t  binding;
    uint16_t offset;

    static constexpr uint16_t InvalidOffset = -1;
};

using VertexLayoutProperties = std::initializer_list<VertexAttribute>;

} // namespace gfx
} // namespace p7