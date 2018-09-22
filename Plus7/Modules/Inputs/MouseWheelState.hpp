#pragma once

namespace p7::inputs {

struct MouseState
{
    struct WheelState
    {
        float vertical   = 0;
        float horizontal = 0;
    };

    struct Position
    {
        float x = 0;
        float y = 0;
    };

    Position   pos;
    WheelState wheel;
    bool       buttons[5] = { false };
};

} // namespace p7::inputs