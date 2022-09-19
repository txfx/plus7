#pragma once

#include "Tasks/Task.hpp"
#include "TypedTask.hpp"

#include <rx/ranges.hpp>
#include <Utils/NonCopyable.hpp>

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace p7::tasks {

struct Pipeline : public NonCopyable
{
    template <typename F>
    auto AddTask(F _functor);

    template <typename F, std::size_t NParent, std::size_t NChild, typename... Ts>
    auto AddTask(TypedTaskDependencies<NParent, NChild, Ts...> _dependencies, F _functor);

    template <typename F>
    auto AddTask(Name _name, F _functor);

    template <typename T1, typename T2, typename... TArgs>
    auto AddTask(Name _name, T1 _dependencies1, T2 _dependencies2, TArgs&&... _args);

    template <typename F, std::size_t NParent, std::size_t NChild, typename... Ts>
    auto AddTask(Name _name, TypedTaskDependencies<NParent, NChild, Ts...> _dependencies, F _functor);

    template <typename T>
    const TaskWithReturn<T>& GetTask(TypedID<T> _id) const;

    const Task& GetTask(ID _id) const { return *tasks[_id]; }

    auto GetTasks() const
    {
        auto deref = [](const auto& t) -> const auto& { return *t; };
        return tasks | rx::transform(deref);
    }

    void Build();
    bool Execute(std::size_t _index) const;
    bool ExecuteWhile(TypedID<bool> _dep) const;

private:
    TaskList tasks;

    bool dirty = true;

    std::vector<ID> executionOrder;
};

template <typename F>
auto Pipeline::AddTask(F _functor)
{
    return AddTask("Anonymous task"_name, NoDependencies(), _functor);
}

template <typename F, std::size_t NParent, std::size_t NChild, typename... Ts>
auto Pipeline::AddTask(TypedTaskDependencies<NParent, NChild, Ts...> _dependencies, F _functor)
{
    return AddTask("Anonymous"_name, _dependencies, _functor);
}

template <typename F>
auto Pipeline::AddTask(Name _name, F _functor)
{
    return AddTask(_name, NoDependencies(), _functor);
}

template <typename T1, typename T2, typename... TArgs>
auto Pipeline::AddTask(Name _name, T1 _dependencies1, T2 _dependencies2, TArgs&&... _args)
{
    return AddTask(_name, _dependencies1.merge(_dependencies2), std::forward<TArgs>(_args)...);
}

template <typename F, std::size_t NParent, std::size_t NChild, typename... Ts>
auto Pipeline::AddTask(Name _name, TypedTaskDependencies<NParent, NChild, Ts...> _dependencies, F _functor)
{
    static_assert(
      std::is_invocable_v<F, Ts...>,
      "Your functor is not callable with the return values from the consumed dependencies");

    using TTask = TypedTask<F, Ts...>;

    TypedID<typename TTask::TReturn> id { static_cast<ID::type>(tasks.size()) };

    auto taskPtr = new TTask { id, _name, _functor, _dependencies, tasks };
    tasks.emplace_back(std::move(taskPtr));

    for (auto parentId : tasks.back()->GetParents())
    {
        if (parentId != self<void>)
            tasks[parentId]->GetChildren().emplace_back(id);
    }

    for (auto childId : tasks.back()->GetChildren())
    {
        if (childId != self<void>)
            tasks[childId]->GetParents().emplace_back(id);
    }

    dirty = true;

    return id;
}

template <typename T>
const TaskWithReturn<T>& Pipeline::GetTask(TypedID<T> _id) const
{
    return static_cast<const TaskWithReturn<T>&>(*tasks[_id.value]);
}

} // namespace p7::tasks