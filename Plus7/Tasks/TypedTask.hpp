#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

#include "Task.hpp"

namespace p7::tasks {

template <typename F, typename... Ts>
struct TypedTask : Task
{
public:
    using TReturn = typename std::invoke_result<F, Ts...>::type;

private:
    friend struct Pipeline;
    friend std::unique_ptr<TypedTask> std::make_unique<TypedTask>(F&);

    template <uint16_t... Ids>
    using id_sequence = std::integer_sequence<uint16_t, Ids...>;

    template <uint16_t N>
    using make_id_sequence = std::make_integer_sequence<uint16_t, N>;

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

} // namespace p7::tasks