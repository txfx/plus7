#pragma once

#include "Dummy/DummyBuffer.hpp"

#include "SdlGl/gl_headers.h"

namespace p7 {
namespace gfx {

struct GlBuffer final : public DummyBuffer
{
    GlBuffer(const BufferProperties& _properties, const void* _data);
    ~GlBuffer() override;

    void UpdateData(uint32_t _offset, uint32_t _size, const void* _data) override;

    GLuint GetGlId() const { return buffer; }

private:
    GLenum type;
    GLuint buffer;
};

} // namespace gfx
} // namespace p7