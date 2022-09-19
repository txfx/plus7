#pragma once

#include <cstdint>
#include <type_traits>

namespace p7::tasks {

template <typename T>
struct TypedID;

struct ID
{
    using type = uint16_t;

    constexpr ID()
        : ID(-1)
    { }

    operator type() const { return value; }

    ID(const ID&)            = default;
    ID& operator=(const ID&) = default;

    ID prev() const { return { value, offset - 1 }; }
    ID next() const { return { value, offset + 1 }; }

    int32_t GetOffset() const { return offset; }

protected:
    constexpr explicit ID(type _id)
        : value(_id)
        , offset(0)
    { }

    constexpr ID(type _id, int32_t _offset)
        : value(_id)
        , offset(_offset)
    { }

    type    value;
    int32_t offset;
};

template <typename T>
struct TypedID : public ID
{
    using ID::ID;

    TypedID<T> prev() const { return { value, offset - 1 }; }
    TypedID<T> next() const { return { value, offset + 1 }; }

protected:
    explicit constexpr TypedID(ID::type _id)
        : ID(_id)
    { }

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
    { }
};

constexpr Name operator"" _name(char const* str, std::size_t len) { return Name(str, len); }

template <typename T>
inline constexpr TypedID<T> self;

} // namespace p7::tasks
