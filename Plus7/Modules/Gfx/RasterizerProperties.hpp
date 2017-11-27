#pragma once

namespace p7 {
namespace gfx {
enum class PolygonMode
{
    Fill,
    Line,
    Point
};

enum class CullMode
{
    None,
    Front,
    Back,
    Front_And_Back
};

enum class FrontFace
{
    ClockWise,
    CounterClockWise
};

struct RasterizerProperties
{
    PolygonMode fillmode  = PolygonMode::Fill;
    CullMode    cullmode  = CullMode::Back;
    FrontFace   frontface = FrontFace::CounterClockWise;
    bool        scissor   = false;
};
} // namespace gfx
} // namespace p7