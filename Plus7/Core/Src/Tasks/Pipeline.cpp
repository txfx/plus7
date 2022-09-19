#include "Tasks/Pipeline.hpp"

#include <rx/ranges.hpp>
#include <Utils/Assert.hpp>

#include <algorithm>

namespace p7::tasks {

bool Pipeline::Execute(std::size_t _index) const
{
    if (dirty)
    {
        return false;
    }

    for (auto taskId : std::as_const(executionOrder))
    {
        tasks[taskId]->Call(_index);
    }

    return true;
}

void Pipeline::Build()
{
    executionOrder.clear();
    executionOrder.reserve(tasks.size());

    std::vector<std::size_t> dependencies(tasks.size(), 0);

    auto isNotSelf = [](auto id) { return id != self<void>; };
    for (auto& task : std::as_const(tasks))
    {
        const auto nbParents = task->GetParents() | rx::filter(isNotSelf) | rx::count();
        if (nbParents == 0)
        {
            executionOrder.emplace_back(task->GetID());
        }
        dependencies[task->GetID()] = nbParents;
    }

    for (uint16_t done = 0; done < tasks.size(); done++)
    {
        auto        taskId = executionOrder[done];
        const auto& task   = tasks[taskId];

        for (auto id : task->GetChildren())
        {
            if (id != self<void>)
            {
                --dependencies[id];
                if (dependencies[id] == 0)
                {
                    executionOrder.push_back(id);
                }
            }
        }
    }

    dirty = false;
}

bool Pipeline::ExecuteWhile(TypedID<bool> _task) const
{
    auto&       whileTask = GetTask(_task);
    bool        valid     = true;
    bool        stop      = false;
    std::size_t index     = 0;
    while (valid && !stop)
    {
        valid = Execute(index++);
        stop  = whileTask.GetReturnValue(0);
    }
    return valid;
}

} // namespace p7::tasks