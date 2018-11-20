#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "TaskDependencies.hpp"
#include "TaskID.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

struct Task : public NonCopyable
{
    virtual ~Task() = default;

    ::std::vector<ID>&       GetParents() { return dependencies.parents; }
    const ::std::vector<ID>& GetParents() const { return dependencies.parents; }

    ::std::vector<ID>&       GetChildren() { return dependencies.children; }
    const ::std::vector<ID>& GetChildren() const { return dependencies.children; }

    auto GetID() const { return id; }
    auto GetName() const { return name; }

private:
    friend struct Pipeline;

    virtual void Call(
        ID                           _returnId,
        ::std::vector<uint8_t>&      _data,
        const ::std::vector<size_t>& _offsets) const = 0;

    virtual size_t GetReturnValueSize() const = 0;

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

} // namespace p7::tasks