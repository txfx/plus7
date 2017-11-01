#pragma once

namespace p7 {
namespace gfx {

enum class BlendFactor
{
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstColor,
    OneMinusConstColor,
    ConstAlpha,
    OneMinusConstAlpha,
    SrcAlphaSaturate,
    Src1Color,
    OneMinusSrc1Color,
    Src1Alpha,
    OneMinusSrc1Alpha
};

enum class BlendOp
{
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

struct BlendProperties
{
    constexpr BlendProperties(BlendFactor _src, BlendFactor _dst, BlendOp _op)
        : enable(true)
        , srcRgb(_src)
        , dstRgb(_dst)
        , opRgb(_op)
        , srcAlpha(_src)
        , dstAlpha(_dst)
        , opAlpha(_op)
    {}

    bool        enable   = false;
    BlendFactor srcRgb   = BlendFactor::One;
    BlendFactor dstRgb   = BlendFactor::Zero;
    BlendOp     opRgb    = BlendOp::Add;
    BlendFactor srcAlpha = BlendFactor::One;
    BlendFactor dstAlpha = BlendFactor::Zero;
    BlendOp     opAlpha  = BlendOp::Add;
};
} // namespace gfx
} // namespace p7