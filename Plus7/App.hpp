#pragma once

#include <Module/Base.hpp>
#include <Utils/NonCopyable.hpp>

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
    void RegisterModule(TArgs... args)
    {
        if (initialized)
        {
            modules.emplace_back(std::make_unique<T>(args..., *this));
        }
    }

    void RequestExit();

private:
    bool initialized    = false;
    bool exit_requested = false;

    std::vector<std::unique_ptr<module::Base>> modules;
}; // namespace p7
} // namespace p7