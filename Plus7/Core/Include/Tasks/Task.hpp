#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "TaskDependencies.hpp"
#include "TaskID.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

struct Task : public NonCopyable
{
    virtual ~Task() = default;

    std::vector<ID>&       GetParents() { return dependencies.parents; }
    const std::vector<ID>& GetParents() const { return dependencies.parents; }

    std::vector<ID>&       GetChildren() { return dependencies.children; }
    const std::vector<ID>& GetChildren() const { return dependencies.children; }

    auto GetID() const { return id; }
    auto GetName() const { return name; }

private:
    friend struct Pipeline;

    virtual void Call(std::size_t _index) = 0;

protected:
    explicit Task(ID _id, Name _name, TaskDependencies _dependencies)
        : id(_id)
        , name(_name)
        , dependencies(std::move(_dependencies))
    {}

    const ID         id;
    const Name       name;
    TaskDependencies dependencies;
};

using TaskList = std::vector<std::unique_ptr<Task>>;

} // namespace p7::tasks