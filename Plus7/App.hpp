#pragma once

#include <Module.hpp>
#include <Tasks/Pipeline.hpp>

namespace p7 {

struct App : public tasks::Pipeline, public ModuleRepository
{
public:
    void Run();

    void RequestExit();

    template <typename T, typename... TArgs>
    T& LoadModule(TArgs&&... args)
    {
        return ModuleRepository::LoadModule<T>(std::forward<TArgs>(args)..., *this);
    }

private:
    bool exit_requested = false;
};

} // namespace p7