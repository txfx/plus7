#pragma once

#include <cstdint>

namespace p7 {
namespace gfx {

struct Buffer
{
    uint32_t GetSize() const;
};

struct BufferSpan
{
    Buffer&       GetBuffer();
    const Buffer& GetBuffer() const;

    uint32_t GetOffset() const;
    uint32_t GetSize() const;
};
} // namespace gfx
} // namespace p7