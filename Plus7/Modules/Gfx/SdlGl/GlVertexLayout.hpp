#pragma once

#include <Modules/Gfx/Dummy/DummyVertexLayout.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {

struct GlVertexLayout final : public DummyVertexLayout
{
    GlVertexLayout(const VertexLayoutProperties& _properties);
    ~GlVertexLayout();

    void Bind() const;

    GLsizei GetStride() const { return stride; }

private:
    GLuint  vao;
    GLsizei stride;
};

} // namespace gfx
} // namespace p7