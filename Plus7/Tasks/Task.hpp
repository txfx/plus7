#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

template <typename T>
struct ID
{
private:
    explicit ID(uint16_t _id)
        : value(_id)
    {}

    friend struct Pipeline;
    template <typename... Ts>
    friend struct After;
    template <typename... Ts>
    friend struct Before;

    uint16_t value;
};

struct Task : public NonCopyable
{
    virtual ~Task() = default;

private:
    using InternalId = uint16_t;

    virtual size_t Call(
        Task::InternalId           _returnId,
        std::vector<uint8_t>&      _data,
        const std::vector<size_t>& _offsets) const = 0;

    friend struct Pipeline;
    template <typename... Ts>
    friend struct After;
    template <typename... Ts>
    friend struct Before;
    template <typename F, typename... Ts>
    friend struct TypedTask;

    std::vector<InternalId> parents;
    std::vector<InternalId> children;
};

template <typename... Ts>
struct After
{
    constexpr explicit After(ID<Ts>... _args)
        : ids({ { _args.value... } })
    {}
    std::array<Task::InternalId, sizeof...(Ts)> ids;
};
template <typename... Ts>
After<Ts...> after(ID<Ts>... _args)
{
    return After<Ts...>{ _args... };
}

template <typename... Ts>
struct Before
{
    constexpr explicit Before(ID<Ts>... _args)
        : ids({ { _args.value... } })
    {}
    std::array<Task::InternalId, sizeof...(Ts)> ids;
};
template <typename... Ts>
Before<Ts...> before(ID<Ts>... _args)
{
    return Before<Ts...>{ _args... };
}

} // namespace p7::tasks