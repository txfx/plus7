#pragma once

#include <Module.hpp>

#include <Tasks/Task.hpp>

namespace p7 {

struct App;

struct SdlApp : public Module
{
public:
    explicit SdlApp(App& _app);
    ~SdlApp() override;

    const tasks::TypedID<void> mainTask;

private:
    void PollEvents();
};

} // namespace p7