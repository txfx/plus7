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

    const tasks::TypedID<MouseState> updateTask;

private:
    MouseState PollEvents();
};

} // namespace inputs
} // namespace p7