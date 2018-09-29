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

    ::std::vector<InternalId>&       GetParents() { return dependencies.parents; }
    const ::std::vector<InternalId>& GetParents() const { return dependencies.parents; }

    ::std::vector<InternalId>&       GetChildren() { return dependencies.children; }
    const ::std::vector<InternalId>& GetChildren() const { return dependencies.children; }

private:
    friend struct Pipeline;

    virtual size_t Call(
        InternalId                   _returnId,
        ::std::vector<uint8_t>&      _data,
        const ::std::vector<size_t>& _offsets) const = 0;

protected:
    explicit Task(TaskDependencies _dependencies)
        : dependencies(_dependencies)
    {}

    TaskDependencies dependencies;
};

} // namespace p7::tasks