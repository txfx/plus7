#pragma once

#include <cstdint>

namespace p7::tasks {

using InternalId = uint16_t;

template <typename T>
struct ID
{
protected:
    explicit ID(InternalId _id)
        : value(_id)
    {}

private:
    template <typename... Ts>
    friend struct TypedTaskDependencies;
    friend struct Pipeline;

    const InternalId value;
};

} // namespace p7::tasks