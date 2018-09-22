#pragma once

#include <Module.hpp>
#include <Tasks/Task.hpp>

#include <Modules/Inputs/KeyboardState.hpp>

namespace p7::inputs {

struct SdlKeyboard : public Module
{
public:
    explicit SdlKeyboard(App& _app);

    auto GetStateTask() const { return getStateTask; }

private:
    KeyboardState PollEvents();

private:
    tasks::ID<KeyboardState> getStateTask;
};

} // namespace p7::inputs