#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "Task.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace tasks {

using TaskList = std::vector<std::unique_ptr<Task>>;

struct Pipeline : public NonCopyable
{
    ~Pipeline();

    ID CreateTask(const Prototype& _functor);
    ID CreateTask(const Prototype& _functor, ID _child);
    ID CreateTask(const Prototype& _functor, const Dependencies& _children);
    ID CreateTask(ID _parent, const Prototype& _functor);
    ID CreateTask(ID _parent, const Prototype& _functor, ID _child);
    ID CreateTask(ID _parent, const Prototype& _functor, const Dependencies& _children);
    ID CreateTask(const Dependencies& _parents, const Prototype& _functor);
    ID CreateTask(const Dependencies& _parents, const Prototype& _functor, ID _child);
    ID CreateTask(const Dependencies& _parents, const Prototype& _functor, const Dependencies& _children);

protected:
    void ExecuteTasks() const;

private:
    TaskList tasks;
};

} // namespace tasks
} // namespace p7