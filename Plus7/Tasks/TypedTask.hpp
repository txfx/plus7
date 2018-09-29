#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

#include "Task.hpp"
#include "TaskDependencies.hpp"

namespace p7::tasks {

template <typename F, typename... Ts>
struct TypedTask : Task
{
public:
    using TReturn = typename std::invoke_result<F, Ts...>::type;

    explicit TypedTask(F _functor, TypedTaskDependencies<Ts...> _dependencies)
        : Task(_dependencies)
        , functor(_functor)
    {}

private:
    friend struct Pipeline;

    using TIndex = uint16_t;

    template <TIndex... Ids>
    using id_sequence = std::integer_sequence<TIndex, Ids...>;

    template <TIndex N>
    using make_id_sequence = std::make_integer_sequence<TIndex, N>;

    size_t Call(
        InternalId                 _returnId,
        std::vector<uint8_t>&      _data,
        const std::vector<size_t>& _offsets) const override
    {
        using seq = make_id_sequence<sizeof...(Ts)>;
        return CallInternal(_returnId, _data, _offsets, seq{});
    }

    template <TIndex... IDs>
    size_t CallInternal(
        [[maybe_unused]] InternalId _returnId,
        std::vector<uint8_t>&       _data,
        const std::vector<size_t>&  _offsets,
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
                *ret = functor(*reinterpret_cast<Ts*>(&_data[_offsets[GetParentID(IDs)]])...);
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
                functor(*reinterpret_cast<Ts*>(&_data[_offsets[GetParentID(IDs)]])...);
            }
            return 0;
        }
    }

    InternalId GetParentID(TIndex index) const
    {
        return dependencies.parents[index];
    }

    const F functor;
};

} // namespace p7::tasks