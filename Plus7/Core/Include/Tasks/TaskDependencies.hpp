#pragma once

#include <array>
#include <vector>

#include <Utils/Array.hpp>

#include "TaskID.hpp"

namespace p7::tasks {

struct TaskDependencies
{
    template <std::size_t NParent, std::size_t NChild>
    TaskDependencies(std::array<ID, NParent> _parents, std::array<ID, NChild> _children)
        : parents(std::begin(_parents), std::end(_parents))
        , children(std::begin(_children), std::end(_children))
    {}

    using vector = std::vector<ID>;

    vector parents;
    vector children;
};

template <std::size_t NParent, std::size_t NChild, typename... Ts>
struct TypedTaskDependencies
{
    template <typename... Us>
    constexpr auto consumes(TypedID<Us>... args) const
    {
        static_assert(sizeof...(Ts) == 0, "You already have specified parent tasks to consume values from.");

        // consummed return values need to be at the front!
        using TReturn = TypedTaskDependencies<NParent + sizeof...(Us), NChild, Us...>;
        return TReturn { concat<ID, sizeof...(Us), NParent>({ args... }, parents), children };
    }

    template <typename... Us>
    constexpr auto triggers(TypedID<Us>... args) const
    {
        using TReturn = TypedTaskDependencies<NParent, NChild + sizeof...(Us), Ts...>;
        return TReturn { parents, concat<ID, NChild, sizeof...(Us)>(children, { args... }) };
    }

    template <typename... Us>
    constexpr auto needs(TypedID<Us>... args) const
    {
        using TReturn = TypedTaskDependencies<NParent + sizeof...(Us), NChild, Ts...>;
        return TReturn { concat<ID, NParent, sizeof...(Us)>(parents, { args... }), children };
    }

    std::array<ID, NParent> parents;
    std::array<ID, NChild>  children;
};

constexpr auto NoDependencies() { return TypedTaskDependencies<0, 0> {}; }

template <typename... Ts>
constexpr auto consumes(TypedID<Ts>... args)
{
    return NoDependencies().consumes(args...);
}

template <typename... Ts>
constexpr auto triggers(TypedID<Ts>... args)
{
    return NoDependencies().triggers(args...);
}

template <typename... Ts>
constexpr auto needs(TypedID<Ts>... args)
{
    return NoDependencies().needs(args...);
}

} // namespace p7::tasks