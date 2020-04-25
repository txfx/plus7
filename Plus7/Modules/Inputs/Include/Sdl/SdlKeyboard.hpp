#pragma once

#include <Tasks/Task.hpp>

#include "KeyboardState.hpp"

namespace p7::tasks {
struct Pipeline;
} // namespace p7::tasks

namespace p7::inputs::sdl {

tasks::TypedID<KeyboardState> CreateUpdateKeyboardTask(tasks::Pipeline& _pipeline, const tasks::ID aSdlMainTask);

} // namespace p7::inputs::sdl