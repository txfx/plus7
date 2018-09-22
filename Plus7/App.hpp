#pragma once

#include <Module.hpp>
#include <Tasks/Pipeline.hpp>

#include <algorithm>
#include <memory>
#include <vector>

namespace p7 {

struct App : public tasks::Pipeline
{
public:
    App();
    ~App();

    void Run();

    template <typename T, typename... TArgs>
    T& LoadModule(TArgs... args);
    template <typename T>
    T& GetDependency();

    auto GetDummyTask() const { return dummyTask; }
    void RequestExit();

private:
    bool exit_requested = false;

    tasks::ID<void> dummyTask;

    std::vector<std::unique_ptr<Module>> modules;
};

//==============================================================================================
// App Implementation
//==============================================================================================
template <typename T, typename... TArgs>
T& App::LoadModule(TArgs... args)
{
    modules.emplace_back(std::make_unique<T>(args..., *this));
    return dynamic_cast<T&>(*modules.back());
}

template <typename T>
T& App::GetDependency()
{
    auto it = std::find_if(modules.begin(), modules.end(), [](const auto& mod) {
        return dynamic_cast<T*>(mod.get()) != nullptr;
    });

    if (it != modules.end())
    {
        return *dynamic_cast<T*>(it->get());
    }
    else
    {
        return LoadModule<T>();
    }
}

} // namespace p7