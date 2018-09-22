#pragma once

#include <Module.hpp>

#include <Tasks/Task.hpp>

namespace p7 {

struct SdlApp : public Module
{
public:
    explicit SdlApp(App& _app);
    ~SdlApp() override;

    auto GetMainTask() const { return mainTask; }

private:
    void PollEvents();

private:
    tasks::ID<void> mainTask;
};

} // namespace p7