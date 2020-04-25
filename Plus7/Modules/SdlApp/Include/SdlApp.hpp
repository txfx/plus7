#pragma once

#include <KeyboardState.hpp>
#include <MouseState.hpp>
#include <Renderer.hpp>
#include <Tasks/Task.hpp>
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {
struct Pipeline;
}

namespace p7::sdl {

struct WindowedApp : NonCopyable
{
    WindowedApp(tasks::Pipeline& _pipeline, const char* _name, int _w, int _h);
    ~WindowedApp();

    const tasks::TypedID<bool>                  mainTask;
    const tasks::TypedID<inputs::MouseState>    mouseTask;
    const tasks::TypedID<inputs::KeyboardState> keyboardTask;
    const std::unique_ptr<gfx::Renderer>        renderer;
};

} // namespace p7::sdl