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

    virtual void BeginFrame() {}
    virtual void Update() = 0;
    virtual void EndFrame() {}

protected:
    App& app;
};
} // namespace p7