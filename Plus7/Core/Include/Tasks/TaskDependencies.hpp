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
    template <std::size_t NParent2, std::size_t NChild2, typename... T2s>
    constexpr auto merge(TypedTaskDependencies<NParent2, NChild2, T2s...> _dependencies)
    {
        static_assert(sizeof...(Ts) == 0 || sizeof...(T2s) == 0, "You cannot merge tasks dependencies which both consumes values.");

        using TReturn = TypedTaskDependencies<NParent + NParent2, NChild + NChild2, Ts..., T2s...>;
        return TReturn {
            // consummed return values need to be at the front!
            concat<ID, NParent2, NParent>(_dependencies.parents, parents),
            concat<ID, NChild, NChild2>(children, _dependencies.children)
        };
    }

    std::array<ID, NParent> parents  = {};
    std::array<ID, NChild>  children = {};
};

constexpr auto NoDependencies() { return TypedTaskDependencies<0, 0> {}; }

template <typename... Ts>
constexpr auto consuming(TypedID<Ts>... args)
{
    using TReturn = TypedTaskDependencies<sizeof...(Ts), 0, Ts...>;
    return TReturn { { args... }, {} };
}

template <typename... Ts>
constexpr auto before(Ts... args)
{
    static_assert((std::is_convertible_v<Ts, ID> && ...), "before can only be called type convertible in ID");
    using TReturn = TypedTaskDependencies<0, sizeof...(Ts)>;
    return TReturn { {}, { args... } };
}

template <typename... Ts>
constexpr auto after(Ts... args)
{
    static_assert((std::is_convertible_v<Ts, ID> && ...), "after can only be called type convertible in ID");
    using TReturn = TypedTaskDependencies<sizeof...(Ts), 0>;
    return TReturn { { args... }, {} };
}

} // namespace p7::tasks