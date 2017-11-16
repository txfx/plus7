#include "GlBuffer.hpp"

namespace p7 {
namespace gfx {

GlBuffer::GlBuffer(uint32_t /* _size */)
{}

GlBuffer::GlBuffer(uint32_t /* _size */, const void* /* _data */)
{}

uint32_t GlBuffer::GetSize() const
{
    return 0;
}

void GlBuffer::UpdateData(uint32_t /* _offset */, uint32_t /* _size */, void* /* _data */)
{
}

} // namespace gfx
} // namespace p7