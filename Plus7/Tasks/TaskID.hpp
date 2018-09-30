#pragma once

#include <cstdint>

namespace p7::tasks {

using InternalId = uint16_t;

template <typename T>
struct ID
{
protected:
    explicit ID(InternalId _id)
        : value(_id)
    {}

private:
    template <typename... Ts>
    friend struct TypedTaskDependencies;
    friend struct Pipeline;

    const InternalId value;
};

struct Name
{
    const char*       value;
    const std::size_t size;

private:
    friend constexpr Name operator"" _name(char const* str, std::size_t);

    constexpr Name(const char* literal, std::size_t len)
        : value(literal)
        , size(len)
    {}
};

constexpr Name operator"" _name(char const* str, std::size_t len)
{
    return Name(str, len);
}

} // namespace p7::tasks