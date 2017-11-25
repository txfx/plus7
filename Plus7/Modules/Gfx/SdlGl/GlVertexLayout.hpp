#pragma once

#include <Modules/Gfx/Dummy/DummyVertexLayout.hpp>

#include "gl_headers.h"

namespace p7 {
namespace gfx {

struct GlVertexLayout final : public DummyVertexLayout
{
    explicit GlVertexLayout(const VertexLayoutProperties& _properties);
    ~GlVertexLayout();

    void Bind() const;

private:
    GLuint vao;
};

} // namespace gfx
} // namespace p7