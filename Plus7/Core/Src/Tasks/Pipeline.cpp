#include "Tasks/Pipeline.hpp"

#include <Utils/Assert.hpp>

namespace p7::tasks {

void Pipeline::ExecuteTasks()
{
    if (dirty)
    {
        dirty = false;
        ComputeExecutionOrder();
    }

    for (auto taskId : std::as_const(executionOrder))
    {
        tasks[taskId]->Call(0);
    }
}

void Pipeline::ComputeExecutionOrder()
{
    executionOrder.clear();
    executionOrder.reserve(tasks.size());

    std::vector<std::size_t> dependencies;
    dependencies.resize(tasks.size());

    for (auto& task : std::as_const(tasks))
    {
        const auto nbParents = task->GetParents().size();
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
            --dependencies[id];
            if (dependencies[id] == 0)
            {
                executionOrder.push_back(id);
            }
        }
    }
}

} // namespace p7::tasks