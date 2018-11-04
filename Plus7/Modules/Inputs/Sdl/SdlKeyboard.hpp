#pragma once

#include <Module.hpp>
#include <Tasks/Task.hpp>

#include <Modules/Inputs/KeyboardState.hpp>

namespace p7 {

struct App;
struct SdlApp;

namespace inputs {

struct SdlKeyboard : public ModuleWithDependencies<SdlApp>
{
public:
    explicit SdlKeyboard(App& _app);

    auto GetStateTask() const { return getStateTask; }

private:
    KeyboardState PollEvents();

private:
    tasks::ID<KeyboardState> getStateTask;
};

} // namespace inputs
} // namespace p7