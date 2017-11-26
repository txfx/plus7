#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace tasks {

struct ID
{
private:
    explicit ID(uint16_t _id)
        : value(_id)
    {}

    friend struct Pipeline;

    uint16_t value;
};

using Prototype    = std::function<void()>;
using Dependencies = std::initializer_list<ID>;

struct Task : public NonCopyable
{
private:
    explicit Task(const Dependencies& _parents, const Prototype& _functor, const Dependencies& _children, ID _id)
        : functor(_functor)
        , id(_id)
        , parents(_parents)
        , children(_children)
    {}

    friend struct Pipeline;

    const Prototype functor;
    const ID        id;

    std::vector<ID> parents;
    std::vector<ID> children;
};

} // namespace tasks
} // namespace p7