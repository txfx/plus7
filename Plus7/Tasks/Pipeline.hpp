#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "TypedTask.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

struct Pipeline : public NonCopyable
{
    // n parents, n children, receive parents return
    template <typename F, typename... Ts>
    auto CreateTask(F _functor, TypedTaskDependencies<Ts...> _dependencies = NoDependencies())
    {
        using TTask = TypedTask<F, Ts...>;

        InternalId id = tasks.size();
        tasks.emplace_back(std::make_unique<TTask>(_functor, _dependencies));

        for (auto parentId : tasks.back()->GetParents())
        {
            tasks[parentId]->GetChildren().push_back(id);
        }

        for (auto childId : tasks.back()->GetChildren())
        {
            tasks[childId]->GetParents().push_back(id);
        }

        using R = typename TTask::TReturn;
        if constexpr (!std::is_same<void, R>::value)
        {
            returnValuesSize += sizeof(R);
        }

        return ID<R>(id);
    }

protected:
    void ExecuteTasks() const;

private:
    std::vector<std::unique_ptr<Task>> tasks;

    size_t returnValuesSize = 0;
};

} // namespace p7::tasks