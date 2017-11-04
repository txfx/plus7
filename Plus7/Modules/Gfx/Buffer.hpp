#pragma once

#include <Modules/Gfx/SdlGl/GlBuffer.hpp>

namespace p7 {
namespace gfx {

using Buffer = GlBuffer;

struct BufferSpan
{
    Buffer&       GetBuffer();
    const Buffer& GetBuffer() const;

    uint32_t GetOffset() const;
    uint32_t GetSize() const;

    void UpdateData(void* _data) { GetBuffer().UpdateData(GetOffset(), GetSize(), _data); }
};
} // namespace gfx
} // namespace p7