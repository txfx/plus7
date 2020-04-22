#pragma once

#include <cstdint>

namespace p7::tasks {

struct ID
{
    using type = uint16_t;

    operator type() const { return value; }

protected:
    constexpr explicit ID(type _id)
        : value(_id)
    {}
    type value;

private:
    // This only exist to please MSVC that cannot construct
    // a zero sized std::array without a default ctor for the
    // element type.
    constexpr ID()
        : ID(0)
    {}
    friend constexpr auto NoDependencies();
    template <typename... Ts>
    friend constexpr auto consuming(Ts...);
    template <typename... Ts>
    friend constexpr auto after(Ts...);
    template <typename... Ts>
    friend constexpr auto before(Ts...);
};

template <typename T>
struct TypedID : public ID
{
protected:
    explicit constexpr TypedID(ID::type _id)
        : ID(_id)
    {}

private:
    template <std::size_t NParent, std::size_t NChild, typename... Ts>
    friend struct TypedTaskDependencies;
    friend struct Pipeline;
};

struct Name
{
    const char*       value;
    const std::size_t size;

private:
    friend constexpr Name operator"" _name(char const* str, std::size_t len);

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