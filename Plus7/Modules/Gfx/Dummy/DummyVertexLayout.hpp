#pragma once

#include <Modules/Gfx/VertexLayoutProperties.hpp>
#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace gfx {

struct DummyVertexLayout : public NonCopyable
{
    DummyVertexLayout(const VertexLayoutProperties& _properties)
        : properties(_properties)
    {}

    VertexLayoutProperties properties;
};

} // namespace gfx
} // namespace p7