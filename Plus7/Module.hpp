#pragma once

#include <Utils/NonCopyable.hpp>

namespace p7 {
struct App;

struct Module : public NonCopyable
{
public:
    explicit Module(App& _app)
        : app(_app)
    {}
    virtual ~Module() = default;

protected:
    App& app;
};
} // namespace p7