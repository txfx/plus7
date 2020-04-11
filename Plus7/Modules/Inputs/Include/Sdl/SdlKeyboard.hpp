#pragma once

#include <Module.hpp>
#include <Tasks/Task.hpp>

#include "KeyboardState.hpp"

namespace p7 {

struct App;
struct SdlApp;

namespace inputs {

struct SdlKeyboard : public ModuleWithDependencies<SdlApp>
{
public:
    explicit SdlKeyboard(App& _app);

    const tasks::TypedID<KeyboardState> updateTask;

private:
    static KeyboardState PollEvents();
};

} // namespace inputs
} // namespace p7