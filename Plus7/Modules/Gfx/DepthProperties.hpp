#pragma once

namespace p7 {
namespace gfx {

enum class DepthCompare
{
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always
};

struct DepthProperties
{
    bool         enable  = false;
    bool         write   = false;
    DepthCompare compare = DepthCompare::Always;
    bool         clip    = true;
    float        min     = 0;
    float        max     = 1;
};
} // namespace gfx
} // namespace p7