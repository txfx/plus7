#include "GlDepthState.hpp"

namespace p7 {
namespace gfx {

namespace {
GLenum ToGl(DepthCompare _mode)
{
    switch (_mode)
    {
    case DepthCompare::Never:
        return GL_NEVER;
    case DepthCompare::Less:
        return GL_LESS;
    case DepthCompare::Equal:
        return GL_EQUAL;
    case DepthCompare::LessOrEqual:
        return GL_LEQUAL;
    case DepthCompare::Greater:
        return GL_GREATER;
    case DepthCompare::NotEqual:
        return GL_NOTEQUAL;
    case DepthCompare::GreaterOrEqual:
        return GL_GEQUAL;
    case DepthCompare::Always:
        return GL_ALWAYS;
    }
}
} // namespace

GlDepthState::GlDepthState(const DepthProperties& _properties)
    : enable(_properties.enable)
    , write(_properties.write)
    , func(ToGl(_properties.compare))
    , clip(_properties.clip)
    , min(_properties.min)
    , max(_properties.max)
{
}

void GlDepthState::Apply() const
{
    if (enable)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(func);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    if (write)
    {
        glDepthMask(GL_TRUE);
        glDepthRangef(min, max);
        if (clip)
        {
            glEnable(GL_DEPTH_CLAMP);
        }
        else
        {
            glDisable(GL_DEPTH_CLAMP);
        }
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
}

} // namespace gfx
} // namespace p7