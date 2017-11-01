#include "GlBlendState.hpp"

namespace p7 {
namespace gfx {
namespace {
GLenum ToGl(BlendFactor _mode)
{
    switch (_mode)
    {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SrcColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::DstColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstColor:
        return GL_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstAlpha:
        return GL_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturate:
        return GL_SRC_ALPHA_SATURATE;
    case BlendFactor::Src1Color:
        return GL_SRC1_COLOR;
    case BlendFactor::OneMinusSrc1Color:
        return GL_ONE_MINUS_SRC1_COLOR;
    case BlendFactor::Src1Alpha:
        return GL_SRC1_ALPHA;
    case BlendFactor::OneMinusSrc1Alpha:
        return GL_ONE_MINUS_SRC1_ALPHA;
    }
}

GLenum ToGl(BlendOp _mode)
{
    switch (_mode)
    {
    case BlendOp::Add:
        return GL_FUNC_ADD;
    case BlendOp::Subtract:
        return GL_FUNC_SUBTRACT;
    case BlendOp::ReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case BlendOp::Min:
        return GL_MIN;
    case BlendOp::Max:
        return GL_MAX;
    }
}

} // namespace

GlBlendState::GlBlendState(const BlendProperties& _properties)
    : enable(_properties.enable)
    , srcRgb(ToGl(_properties.srcRgb))
    , dstRgb(ToGl(_properties.dstRgb))
    , opRgb(ToGl(_properties.opRgb))
    , srcAlpha(ToGl(_properties.srcAlpha))
    , dstAlpha(ToGl(_properties.dstAlpha))
    , opAlpha(ToGl(_properties.opAlpha))
{}

void GlBlendState::Apply() const
{
    if (enable)
    {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(srcRgb, dstRgb, srcAlpha, dstAlpha);
        glBlendEquationSeparate(opRgb, opAlpha);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}
} // namespace gfx
} // namespace p7