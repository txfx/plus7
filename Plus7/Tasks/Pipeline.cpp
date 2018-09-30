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

    for (auto taskId : ::std::as_const(runningOrder))
    {
        const auto& task = tasks[taskId];
        task->Call(taskId, returnValues, returnValuesOffset);
    }
}

void Pipeline::ComputeExecutionOrder()
{
    runningOrder.clear();
    runningOrder.reserve(tasks.size());
    returnValuesOffset.resize(tasks.size());

    std::vector<uint16_t> dependencies;
    dependencies.resize(tasks.size());

    for (InternalId id = 0; id < tasks.size(); ++id)
    {
        const uint16_t nbParents = tasks[id]->GetParents().size();
        if (nbParents == 0)
        {
            runningOrder.push_back(id);
        }
        dependencies[id] = nbParents;
    }

    size_t offset = 0;
    for (uint16_t done = 0; done < tasks.size(); done++)
    {
        auto        taskId = runningOrder[done];
        const auto& task   = tasks[taskId];

        returnValuesOffset[taskId] = offset;
        offset += task->GetReturnValueSize();
        for (auto id : task->GetChildren())
        {
            --dependencies[id];
            if (dependencies[id] == 0)
            {
                runningOrder.push_back(id);
            }
        }
    }
    returnValuesSize = offset;
}

} // namespace p7::tasks