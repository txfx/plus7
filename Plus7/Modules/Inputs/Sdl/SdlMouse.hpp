#pragma once

#include <Module.hpp>
#include <Tasks/Task.hpp>

#include <Modules/Inputs/MouseState.hpp>

namespace p7 {

struct App;
struct SdlApp;

namespace inputs {

struct SdlMouse : public ModuleWithDependencies<SdlApp>
{
public:
    explicit SdlMouse(App& _app);

    auto GetStateTask() const { return getStateTask; }

private:
    MouseState PollEvents();

private:
    tasks::TypedID<MouseState> getStateTask;
};

} // namespace inputs
} // namespace p7