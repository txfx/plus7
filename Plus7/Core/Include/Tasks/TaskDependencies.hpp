#pragma once

#include <array>
#include <vector>

#include "TaskID.hpp"

namespace p7::tasks {

struct TaskDependencies
{
    using vector = std::vector<ID>;

    vector parents;
    vector children;
};

template <typename... Ts>
struct TypedTaskDependencies : TaskDependencies
{
    template <typename... Us>
    auto consumes(TypedID<Us>... args) const
    {
        static_assert(sizeof...(Ts) == 0, "You already have specified parent tasks to consume values from.");

        //we create a new TypedTasDependencies with our previous children
        TypedTaskDependencies<Us...> deps { { {}, children } };
        // we add our consumed tasks in the front
        deps.parents.reserve(parents.size() + sizeof...(Us));
        deps.parents.insert(std::end(deps.parents), { args... });
        // we then add back our needed tasks
        deps.parents.insert(std::end(deps.parents), std::begin(parents), std::end(parents));

        return deps;
    }

    template <typename... Us>
    auto& triggers(TypedID<Us>... args)
    {
        children.reserve(children.size() + sizeof...(Us));
        children.insert(std::end(children), { args... });
        return *this;
    }

    template <typename... Us>
    auto& needs(TypedID<Us>... args)
    {
        parents.reserve(parents.size() + sizeof...(Us));
        parents.insert(std::end(parents), { args... });
        return *this;
    }
};

inline auto NoDependencies() { return TypedTaskDependencies<> {}; }

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