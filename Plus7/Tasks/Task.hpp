#pragma once

#include <cstdint>
#include <tuple>
#include <utility>
#include <vector>

#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace tasks {

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
    template <typename R, typename Ts, typename IDs, typename F>
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

template <uint16_t... Ids>
using id_sequence = std::integer_sequence<uint16_t, Ids...>;

template <uint16_t N>
using make_id_sequence = std::make_integer_sequence<uint16_t, N>;

template <typename R, typename Ts, typename IDs, typename F>
struct TypedTask
{
};

template <typename R, typename... Ts, uint16_t... IDs, typename F>
struct TypedTask<R, std::tuple<Ts...>, id_sequence<IDs...>, F> : Task
{
private:
    friend struct Pipeline;

    TypedTask(F _functor)
        : functor(_functor)
    {}

    size_t Call(
        Task::InternalId           _returnId,
        std::vector<uint8_t>&      _data,
        const std::vector<size_t>& _offsets) const override
    {
        R* ret = reinterpret_cast<R*>(&_data[_offsets[_returnId]]);

        if constexpr ((std::is_same<void, Ts>::value && ...))
        {
            *ret = functor();
        }
        else
        {
            *ret = functor(*reinterpret_cast<Ts*>(&_data[_offsets[parents[IDs]]])...);
        }

        return sizeof(R);
    }

    const F functor;
};

template <typename... Ts, uint16_t... IDs, typename F>
struct TypedTask<void, std::tuple<Ts...>, id_sequence<IDs...>, F> : Task
{
private:
    friend struct Pipeline;

    TypedTask(F _functor)
        : functor(_functor)
    {}

    size_t Call(
        Task::InternalId /* _returnId */,
        std::vector<uint8_t>&      _data,
        const std::vector<size_t>& _offsets) const override
    {
        if constexpr ((std::is_same<void, Ts>::value && ...))
        {
            functor();
        }
        else
        {
            functor(*reinterpret_cast<Ts*>(&_data[_offsets[parents[IDs]]])...);
        }
        return 0;
    }

    const F functor;
};

} // namespace tasks
} // namespace p7