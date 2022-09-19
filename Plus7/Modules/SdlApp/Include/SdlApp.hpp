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
    struct FrameInfo
    {
        uint32_t index         = 0;
        uint32_t startTime     = 0;
        uint32_t endTime       = 0;
        bool     exitRequested = false;
    };

    WindowedApp(tasks::Pipeline& _pipeline, const char* _name, int _w, int _h);
    ~WindowedApp();

    const tasks::TypedID<FrameInfo>             mainTask;
    const tasks::TypedID<inputs::MouseState>    mouseTask;
    const tasks::TypedID<inputs::KeyboardState> keyboardTask;
    const std::unique_ptr<gfx::Renderer>        renderer;
};

} // namespace p7::sdl