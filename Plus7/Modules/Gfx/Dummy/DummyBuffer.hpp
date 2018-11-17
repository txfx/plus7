#pragma once

#include <Modules/Gfx/BufferProperties.hpp>
#include <Utils/NonCopyable.hpp>

#include <cstdint>

namespace p7 {
namespace gfx {

struct DummyBuffer : public NonCopyable
{
    DummyBuffer(const BufferProperties& _properties, const void* /* _data */)
        : properties(_properties)
    {}

    virtual ~DummyBuffer() = default;

    virtual uint32_t GetSize() const { return properties.size; }

    virtual void UpdateData(uint32_t /* _offset */, uint32_t /* _size */, const void* /* _data */) {}

private:
    BufferProperties properties;
};

} // namespace gfx
} // namespace p7