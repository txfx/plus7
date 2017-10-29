#pragma once

namespace p7 {
struct NonCopyable
{
private:
    NonCopyable(const NonCopyable&) = delete;
    const NonCopyable& operator=(const NonCopyable&) = delete;
};
} // namespace p7