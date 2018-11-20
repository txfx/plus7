#pragma once

#include <array>
#include <vector>

#include "TaskID.hpp"

namespace p7::tasks {

struct TaskDependencies
{
    using vector = ::std::vector<ID>;

    vector parents;
    vector children;
};

template <typename... Ts>
struct TypedTaskDependencies : TaskDependencies
{
    template <typename... Us>
    auto consume(TypedID<Us>... args) const
    {
        static_assert(sizeof...(Ts) == 0, "You already have specified parent tasks to consume values from.");

        return TypedTaskDependencies<Us...> { { parents, children } }.run_after(args...);
    }

    template <typename... Us>
    auto& run_before(TypedID<Us>... args)
    {
        children.reserve(children.size() + sizeof...(Us));
        children.insert(::std::end(children), { args... });
        return *this;
    }

    template <typename... Us>
    auto& run_after(TypedID<Us>... args)
    {
        parents.reserve(parents.size() + sizeof...(Us));
        parents.insert(::std::end(parents), { args... });
        return *this;
    }
};

inline auto NoDependencies() { return TypedTaskDependencies<> {}; }

template <typename... Ts>
constexpr auto consume(TypedID<Ts>... args)
{
    return NoDependencies().consume(args...);
}

template <typename... Ts>
constexpr auto run_before(TypedID<Ts>... args)
{
    return NoDependencies().run_before(args...);
}

template <typename... Ts>
constexpr auto run_after(TypedID<Ts>... args)
{
    return NoDependencies().run_after(args...);
}

} // namespace p7::tasks