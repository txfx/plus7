#pragma once

#include <Module.hpp>
#include <Tasks/Task.hpp>

#include <Modules/Inputs/MouseState.hpp>

namespace p7::inputs {

struct SdlMouse : public Module
{
public:
    explicit SdlMouse(App& _app);

    auto GetStateTask() const { return getStateTask; }

private:
    MouseState PollEvents();

private:
    tasks::ID<MouseState> getStateTask;
};

} // namespace p7::inputs