#include "Pipeline.hpp"

#include <Utils/Assert.hpp>

namespace p7 {
namespace tasks {

void Pipeline::ExecuteTasks() const
{
    std::vector<Task::InternalId> todo;
    todo.reserve(tasks.size());

    std::vector<uint16_t> dependencies;
    dependencies.resize(tasks.size());

    std::vector<size_t> returnValuesOffset;
    returnValuesOffset.resize(tasks.size());

    std::vector<uint8_t> returnValues;
    returnValues.resize(returnValuesSize);

    for (uint16_t i = 0; i < tasks.size(); ++i)
    {
        const uint16_t nbParents = tasks[i]->parents.size();
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
        for (auto id : task->children)
        {
            --dependencies[id];
            if (dependencies[id] == 0)
            {
                todo.push_back(id);
            }
        }
    }
}

} // namespace tasks
} // namespace p7