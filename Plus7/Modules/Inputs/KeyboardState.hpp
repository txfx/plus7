#pragma once

#include <array>

namespace p7::inputs {

struct KeyboardState
{
    std::array<char, 32>  text     = { 0 };
    std::array<bool, 512> keysDown = { false };

    bool shiftMod = false;
    bool ctrlMod  = false;
    bool altMod   = false;
    bool superMod = false;
};

} // namespace p7::inputs