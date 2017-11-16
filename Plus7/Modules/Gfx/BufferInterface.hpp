#pragma once

#include <Utils/NonCopyable.hpp>

#include <cstdint>

namespace p7 {
namespace gfx {

struct BufferInterface : public NonCopyable
{
    virtual uint32_t GetSize() const = 0;

    virtual void UpdateData(uint32_t _offset, uint32_t _size, void* _data) = 0;
};

} // namespace gfx
} // namespace p7