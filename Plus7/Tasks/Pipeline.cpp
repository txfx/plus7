#include "Pipeline.hpp"

#include <Utils/Assert.hpp>

namespace p7::tasks {

void Pipeline::ExecuteTasks() const
{
    std::vector<InternalId> todo;
    todo.reserve(tasks.size());

    std::vector<uint16_t> dependencies;
    dependencies.resize(tasks.size());

    std::vector<size_t> returnValuesOffset;
    returnValuesOffset.resize(tasks.size());

    std::vector<uint8_t> returnValues;
    returnValues.resize(returnValuesSize);

    for (InternalId i = 0; i < tasks.size(); ++i)
    {
        const uint16_t nbParents = tasks[i]->GetParents().size();
        if (nbParents == 0)
        {
            todo.push_back(i);
        }
        dependencies[i] = nbParents;
    }

    size_t offset = 0;
    while (!todo.empty())
    {
        auto taskId = todo.back();
        todo.pop_back();
        const auto& task = tasks[taskId];

        returnValuesOffset[taskId] = offset;
        offset += task->Call(taskId, returnValues, returnValuesOffset);
        for (auto id : task->GetChildren())
        {
            --dependencies[id];
            if (dependencies[id] == 0)
            {
                todo.push_back(id);
            }
        }
    }
}

} // namespace p7::tasks