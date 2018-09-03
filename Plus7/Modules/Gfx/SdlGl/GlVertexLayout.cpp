#include "GlVertexLayout.hpp"

#include <Utils/Assert.hpp>

namespace p7 {
namespace gfx {

namespace {
struct GLVertexAttribute
{
    GLenum    type;
    GLuint    count;
    GLboolean interpolate;
};

GLVertexAttribute ToGl(VertexAttribute::Type _type)
{
    switch (_type)
    {
    case VertexAttribute::Type::R8G8B8A8_UNORM:
        return { GL_UNSIGNED_BYTE, 4, GL_TRUE };
    case VertexAttribute::Type::XYZW32F:
        return { GL_FLOAT, 4, GL_FALSE };
    default:
        P7_ASSERT(false, "Unknown GL Type %d", _type);
        return { GL_FLOAT, 0, GL_FALSE };
    }
}

GLsizei GlTypeSize(GLenum _type)
{
    switch (_type)
    {
    case GL_FLOAT:
        return 4;
    case GL_UNSIGNED_BYTE:
        return 1;
    default:
        P7_ASSERT(false, "Unknown GL Type %d", _type);
        return -1;
    }
}

GLuint glGenVertexArray()
{
    GLuint vao = -1;
    glGenVertexArrays(1, &vao);
    return vao;
}

} // namespace

GlVertexLayout::GlVertexLayout(const VertexLayoutProperties& _properties)
    : DummyVertexLayout(_properties)
    , vao(glGenVertexArray())
{
    // VAO
    glBindVertexArray(vao);
    GLuint  index  = 0;
    GLsizei offset = 0;
    for (const auto& attrib : _properties)
    {
        const auto    glAttrib     = ToGl(attrib.type);
        const GLsizei attribOffset = attrib.offset != VertexAttribute::InvalidOffset ? attrib.offset : offset;

        glEnableVertexAttribArray(index);
        glVertexAttribFormat(index, glAttrib.count, glAttrib.type, glAttrib.interpolate, attribOffset);
        glVertexAttribBinding(index, attrib.binding);

        ++index;
        offset = attribOffset + glAttrib.count * GlTypeSize(glAttrib.type);
    }
    glBindVertexArray(0);

    stride = offset;
}

GlVertexLayout::~GlVertexLayout()
{
    glDeleteVertexArrays(1, &vao);
}

void GlVertexLayout::Bind() const
{
    glBindVertexArray(vao);
}

} // namespace gfx
} // namespace p7