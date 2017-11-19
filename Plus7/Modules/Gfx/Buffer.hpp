#pragma once

#include <Modules/Gfx/SdlGl/GlBuffer.hpp>
#include <memory>

namespace p7 {
namespace gfx {

using Buffer    = GlBuffer;
using BufferPtr = std::shared_ptr<Buffer>;

struct BufferSpan
{
    BufferSpan(BufferPtr& _buffer, uint32_t _offset, uint32_t _size)
        : buffer(_buffer)
        , offset(_offset)
        , size(_size)
    {}

    BufferSpan(BufferPtr&& _buffer, uint32_t _offset, uint32_t _size)
        : buffer(_buffer)
        , offset(_offset)
        , size(_size)
    {}

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