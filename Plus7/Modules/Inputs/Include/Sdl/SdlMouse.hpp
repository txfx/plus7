#pragma once

#include <Tasks/Task.hpp>

#include "MouseState.hpp"

namespace p7::tasks {
struct Pipeline;
} // namespace p7::tasks

namespace p7::inputs::sdl {

tasks::TypedID<MouseState> CreateUpdateMouseTask(tasks::Pipeline& _pipeline, const tasks::ID aSdlMainTask);

} // namespace p7::inputs::sdl