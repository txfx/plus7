#pragma once

#include <Utils/NonCopyable.hpp>

namespace p7 {
struct App;

namespace module {
struct Base : public NonCopyable
{
public:
    explicit Base(App& _app)
        : app(_app)
    {}
    virtual ~Base() = default;

    virtual void Update() = 0;

protected:
    App& app;
};
} // namespace module
} // namespace p7