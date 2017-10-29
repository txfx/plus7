#pragma once

namespace p7 {
struct NonCopyable
{
protected:
    NonCopyable()  = default;
    ~NonCopyable() = default;

    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;

public:
    NonCopyable(const NonCopyable&) = delete;
    const NonCopyable& operator=(const NonCopyable&) = delete;
};
} // namespace p7