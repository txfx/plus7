#pragma once

#include "utils/NonCopyable.hpp"

namespace p7 {
struct App : public NonCopyable
{
public:
    App()  = default;
    ~App() = default;

    void Run();
}; // namespace p7
} // namespace p7