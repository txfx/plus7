#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "TypedTask.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

struct Pipeline : public NonCopyable
{
    template <typename F>
    auto AddTask(F _functor);

    template <typename F, typename... Ts>
    auto AddTask(TypedTaskDependencies<Ts...> _dependencies, F _functor);

    template <typename F>
    auto AddTask(Name _name, F _functor);

    template <typename F, typename... Ts>
    auto AddTask(Name _name, TypedTaskDependencies<Ts...> _dependencies, F _functor);

    const auto& GetTasks() const { return tasks; }

protected:
    void ExecuteTasks();

    void ComputeExecutionOrder();

private:
    std::vector<std::unique_ptr<Task>> tasks;

    bool   dirty            = true;
    size_t returnValuesSize = 0;

    std::vector<ID>     executionOrder;
    std::vector<size_t> returnValuesOffset;
};

template <typename F>
auto Pipeline::AddTask(F _functor)
{
    return AddTask("Anonymous task"_name, NoDependencies(), _functor);
}

template <typename F, typename... Ts>
auto Pipeline::AddTask(TypedTaskDependencies<Ts...> _dependencies, F _functor)
{
    return AddTask("Anonymous"_name, _dependencies, _functor);
}

template <typename F>
auto Pipeline::AddTask(Name _name, F _functor)
{
    return AddTask(_name, NoDependencies(), _functor);
}

template <typename F, typename... Ts>
auto Pipeline::AddTask(Name _name, TypedTaskDependencies<Ts...> _dependencies, F _functor)
{
    static_assert(::std::is_invocable<F, Ts...>::value,
        "Your functor is not callablee with the return values from the consume dependencies");

    using TTask = TypedTask<F, Ts...>;

    TypedID<typename TTask::TReturn> id { static_cast<ID::type>(tasks.size()) };
    tasks.emplace_back(std::make_unique<TTask>(id, _name, _functor, _dependencies));

    for (auto parentId : tasks.back()->GetParents())
    {
        tasks[parentId]->GetChildren().emplace_back(id);
    }

    for (auto childId : tasks.back()->GetChildren())
    {
        tasks[childId]->GetParents().emplace_back(id);
    }

    dirty = true;

    return id;
}

} // namespace p7::tasks