#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "TypedTask.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

struct Pipeline : public NonCopyable
{
    template <typename F, typename... Ts>
    auto CreateTask(F _functor, TypedTaskDependencies<Ts...> _dependencies = NoDependencies());

    template <typename F, typename... Ts>
    auto CreateTask(Name _name, F _functor, TypedTaskDependencies<Ts...> _dependencies = NoDependencies());

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

template <typename F, typename... Ts>
auto Pipeline::CreateTask(F _functor, TypedTaskDependencies<Ts...> _dependencies)
{
    return CreateTask("Anonymous"_name, _functor, _dependencies);
}

template <typename F, typename... Ts>
auto Pipeline::CreateTask(Name _name, F _functor, TypedTaskDependencies<Ts...> _dependencies)
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