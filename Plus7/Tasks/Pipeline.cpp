#include "Pipeline.hpp"

#include <Utils/Assert.hpp>

namespace p7::tasks {

void Pipeline::ExecuteTasks()
{
    if (dirty)
    {
        dirty = false;
        ComputeExecutionOrder();
    }

    std::vector<uint8_t> returnValues;
    returnValues.resize(returnValuesSize);

    for (auto taskId : ::std::as_const(executionOrder))
    {
        const auto& task = tasks[taskId];
        task->Call(taskId, returnValues, returnValuesOffset);
    }
}

void Pipeline::ComputeExecutionOrder()
{
    executionOrder.clear();
    executionOrder.reserve(tasks.size());
    returnValuesOffset.resize(tasks.size());

    std::vector<std::size_t> dependencies;
    dependencies.resize(tasks.size());

    //for (ID::type id = 0; id < tasks.size(); ++id)
    for (auto& task : std::as_const(tasks))
    {
        const auto nbParents = task->GetParents().size();
        if (nbParents == 0)
        {
            executionOrder.emplace_back(task->GetID());
        }
        dependencies[task->GetID()] = nbParents;
    }

    size_t offset = 0;
    for (uint16_t done = 0; done < tasks.size(); done++)
    {
        auto        taskId = executionOrder[done];
        const auto& task   = tasks[taskId];

        returnValuesOffset[taskId] = offset;
        offset += task->GetReturnValueSize();
        for (auto id : task->GetChildren())
        {
            --dependencies[id];
            if (dependencies[id] == 0)
            {
                executionOrder.push_back(id);
            }
        }
    }
    returnValuesSize = offset;
}

} // namespace p7::tasks