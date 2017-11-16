#pragma once

#include <Modules/Gfx/SdlGl/GlBuffer.hpp>
#include <memory>

namespace p7 {
namespace gfx {

using BufferPtr = std::shared_ptr<GlBuffer>;

struct BufferSpan
{
    BufferPtr&       GetBuffer() { return buffer; }
    const BufferPtr& GetBuffer() const { return buffer; }

    uint32_t GetOffset() const { return offset; }
    uint32_t GetSize() const { return size; }

    void UpdateData(void* _data) { GetBuffer()->UpdateData(GetOffset(), GetSize(), _data); }

private:
    BufferPtr buffer;
    uint32_t  offset;
    uint32_t  size;
};
} // namespace gfx
} // namespace p7