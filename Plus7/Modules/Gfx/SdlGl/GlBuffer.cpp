#include "GlBuffer.hpp"

#include <Utils/Assert.hpp>

namespace p7 {
namespace gfx {

namespace {
GLenum ToGl(BufferType _type)
{
    switch (_type)
    {
    case BufferType::Vertex:
        return GL_ARRAY_BUFFER;
    case BufferType::Index:
        return GL_ELEMENT_ARRAY_BUFFER;
    case BufferType::Constant:
        return GL_UNIFORM_BUFFER;
    default:
        P7_ASSERT(false, "Unknown GL type %d", _type);
        return -1;
    }
}
} // namespace

GlBuffer::GlBuffer(const BufferProperties& _properties, const void* _data)
    : DummyBuffer(_properties, _data)
    , type(ToGl(_properties.type))
{
    glGenBuffers(1, &buffer);
    UpdateData(0, GetSize(), _data);
}

GlBuffer::~GlBuffer()
{
    glDeleteBuffers(1, &buffer);
}

void GlBuffer::UpdateData(uint32_t /* _offset */, uint32_t /* _size */, const void* _data)
{
    glBindBuffer(type, buffer);
    glBufferData(type, GetSize(), _data, GL_DYNAMIC_DRAW);
    glBindBuffer(type, 0);
}

} // namespace gfx
} // namespace p7