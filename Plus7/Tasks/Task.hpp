#pragma once

#include <cstdint>
#include <memory>
#include <tuple>
#include <type_traits>
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

template <uint16_t... Ids>
using id_sequence = std::integer_sequence<uint16_t, Ids...>;

template <uint16_t N>
using make_id_sequence = std::make_integer_sequence<uint16_t, N>;

template <typename F, typename... Ts>
struct TypedTask : Task
{
public:
    using TReturn = typename std::invoke_result<F, Ts...>::type;

private:
    friend struct Pipeline;
    friend std::unique_ptr<TypedTask> std::make_unique<TypedTask>(F&);

    explicit TypedTask(F _functor)
        : functor(_functor)
    {}

    size_t Call(
        Task::InternalId           _returnId,
        std::vector<uint8_t>&      _data,
        const std::vector<size_t>& _offsets) const override
    {
        using seq = make_id_sequence<sizeof...(Ts)>;
        return CallInternal(_returnId, _data, _offsets, seq{});
    }

    template <uint16_t... IDs>
    size_t CallInternal(
        [[maybe_unused]] Task::InternalId _returnId,
        std::vector<uint8_t>&             _data,
        const std::vector<size_t>&        _offsets,
        id_sequence<IDs...>) const
    {
        constexpr bool has_arguments = ((!std::is_same<void, Ts>::value || ...));
        constexpr bool has_return    = !(std::is_same<void, TReturn>::value);

        if constexpr (has_return)
        {
            auto ret = reinterpret_cast<TReturn*>(&_data[_offsets[_returnId]]);
            if constexpr (!has_arguments)
            {
                *ret = functor();
            }
            else
            {
                *ret = functor(*reinterpret_cast<Ts*>(&_data[_offsets[parents[IDs]]])...);
            }
            return sizeof(TReturn);
        }
        else
        {
            if constexpr (!has_arguments)
            {
                functor();
            }
            else
            {
                functor(*reinterpret_cast<Ts*>(&_data[_offsets[parents[IDs]]])...);
            }
            return 0;
        }
    }

    const F functor;
};

} // namespace tasks
} // namespace p7