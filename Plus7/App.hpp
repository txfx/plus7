#pragma once

#include <Module/Base.hpp>
#include <Utils/NonCopyable.hpp>

#include <algorithm>
#include <memory>
#include <vector>

namespace p7 {

struct App : public NonCopyable
{
public:
    App();
    ~App();

    void Run();

    template <typename T, typename... TArgs>
    T& LoadModule(TArgs... args)
    {
        modules.emplace_back(std::make_unique<T>(args..., *this));
        return dynamic_cast<T&>(*modules.back().get());
    }

    template <typename T>
    T& GetDependency()
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

    void RequestExit();

private:
    bool exit_requested = false;

    std::vector<std::unique_ptr<module::Base>> modules;
}; // namespace p7
} // namespace p7