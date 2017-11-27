#include "GlRasterizerState.hpp"

namespace p7 {
namespace gfx {

namespace {
GLenum ToGl(PolygonMode _mode)
{
    switch (_mode)
    {
    case PolygonMode::Fill:
        return GL_FILL;
    case PolygonMode::Line:
        return GL_LINE;
    case PolygonMode::Point:
        return GL_POINT;
    }
}

GLenum ToGl(CullMode _mode)
{
    switch (_mode)
    {
    case CullMode::None:
        return GL_FRONT;
    case CullMode::Front:
        return GL_FRONT;
    case CullMode::Back:
        return GL_BACK;
    case CullMode::Front_And_Back:
        return GL_FRONT_AND_BACK;
    }
}

GLenum ToGl(FrontFace _face)
{
    switch (_face)
    {
    case FrontFace::ClockWise:
        return GL_CW;
    case FrontFace::CounterClockWise:
        return GL_CCW;
    }
}

} // namespace

GlRasterizerState::GlRasterizerState(const RasterizerProperties& _properties)
    : fillmode(ToGl(_properties.fillmode))
    , cullmode(ToGl(_properties.cullmode))
    , frontface(ToGl(_properties.frontface))
    , cull(_properties.cullmode != CullMode::None)
    , scissor(_properties.scissor)
{}

void GlRasterizerState::Bind() const
{
    if (cull)
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(frontface);
        glCullFace(cullmode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, fillmode);

    if (scissor)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

} // namespace gfx
} // namespace p7