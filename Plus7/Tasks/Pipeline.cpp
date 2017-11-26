#include "Pipeline.hpp"

namespace p7 {
namespace tasks {

Pipeline::~Pipeline()
{
    tasks.clear();
}

ID Pipeline::CreateTask(const Prototype& _functor)
{
    return CreateTask({}, _functor, {});
}

ID Pipeline::CreateTask(const Prototype& _functor, ID _child)
{
    return CreateTask({}, _functor, { _child });
}

ID Pipeline::CreateTask(const Prototype& _functor, const Dependencies& _children)
{
    return CreateTask({}, _functor, _children);
}

ID Pipeline::CreateTask(ID _parent, const Prototype& _functor)
{
    return CreateTask({ _parent }, _functor, {});
}

ID Pipeline::CreateTask(ID _parent, const Prototype& _functor, ID _child)
{
    return CreateTask({ _parent }, _functor, { _child });
}

ID Pipeline::CreateTask(ID _parent, const Prototype& _functor, const Dependencies& _children)
{
    return CreateTask({ _parent }, _functor, _children);
}

ID Pipeline::CreateTask(const Dependencies& _parents, const Prototype& _functor)
{
    return CreateTask(_parents, _functor, {});
}

ID Pipeline::CreateTask(const Dependencies& _parents, const Prototype& _functor, ID _child)
{
    return CreateTask(_parents, _functor, { _child });
}

ID Pipeline::CreateTask(const Dependencies& _parents, const Prototype& _functor, const Dependencies& _children)
{
    Task task = { _parents, _functor, _children, ID(tasks.size()) };
    for (auto parent : _parents)
    {
        tasks[parent.value].children.push_back(task.id);
    }
    for (auto child : _children)
    {
        tasks[child.value].parents.push_back(task.id);
    }
    tasks.emplace_back(std::move(task));
    return tasks.back().id;
}

void Pipeline::ExecuteTasks() const
{
    std::vector<const Task*> todo;
    todo.reserve(tasks.size());

    std::vector<uint16_t> dependencies;
    dependencies.reserve(tasks.size());

    for (uint16_t i = 0; i < tasks.size(); ++i)
    {
        const uint16_t nbParents = tasks[i].parents.size();
        if (nbParents == 0)
        {
            todo.push_back(&tasks[i]);
        }
        dependencies[i] = nbParents;
    }

    while (!todo.empty())
    {
        auto task = todo.back();
        todo.pop_back();
        task->functor();
        for (auto id : task->children)
        {
            --dependencies[id.value];
            if (dependencies[id.value] == 0)
            {
                todo.push_back(&tasks[id.value]);
            }
        }
    }
}

} // namespace tasks
} // namespace p7