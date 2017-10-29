#pragma once

#include <utils/NonCopyable.hpp>

namespace p7 {
struct App : public NonCopyable
{
public:
    App();
    ~App();

    void Run();

private:
    bool initialized = false;
}; // namespace p7
} // namespace p7