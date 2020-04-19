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
    template <std::size_t NParent, std::size_t NChild, typename... Ts>
    explicit Task(ID _id, Name _name, TypedTaskDependencies<NParent, NChild, Ts...> _dependencies)
        : id(_id)
        , name(_name)
        , dependencies({ _dependencies.parents, _dependencies.children })
    {}

    const ID         id;
    const Name       name;
    TaskDependencies dependencies;
};

using TaskList = std::vector<std::unique_ptr<Task>>;

} // namespace p7::tasks