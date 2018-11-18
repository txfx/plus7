#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

#include "Task.hpp"
#include "TaskDependencies.hpp"

namespace p7::tasks {

template <typename F, typename... Ts>
struct TypedTask final : Task
{
public:
    using TReturn = typename std::invoke_result<F, Ts...>::type;

    explicit TypedTask(Name _name, F _functor, TypedTaskDependencies<Ts...> _dependencies)
        : Task(_name, _dependencies)
        , functor(_functor)
    {}

private:
    friend struct Pipeline;

    using TIndex = uint16_t;

    template <TIndex... Ids>
    using id_sequence = std::integer_sequence<TIndex, Ids...>;

    template <TIndex N>
    using make_id_sequence = std::make_integer_sequence<TIndex, N>;

    static constexpr bool has_arguments = ((!std::is_same<void, Ts>::value || ...));
    static constexpr bool has_return    = !(std::is_same<void, TReturn>::value);

    void Call(
        InternalId                 _returnId,
        std::vector<uint8_t>&      _data,
        const std::vector<size_t>& _offsets) const override
    {
        using seq = make_id_sequence<sizeof...(Ts)>;
        return CallInternal(_returnId, _data, _offsets, seq {});
    }

    template <TIndex... IDs>
    void CallInternal(
        [[maybe_unused]] InternalId _returnId,
        std::vector<uint8_t>&       _data,
        const std::vector<size_t>&  _offsets,
        id_sequence<IDs...>) const
    {
        if constexpr (has_return)
        {
            auto ret = ObjectFromId<TReturn>(_returnId, _data, _offsets);
            if constexpr (!has_arguments)
            {
                *ret = functor();
            }
            else
            {
                *ret = functor(*GetParameter<Ts>(IDs, _data, _offsets)...);
            }
        }
        else
        {
            if constexpr (!has_arguments)
            {
                functor();
            }
            else
            {
                functor(*GetParameter<Ts>(IDs, _data, _offsets)...);
            }
        }
    }

    template <typename T>
    T* ObjectFromId(InternalId id, std::vector<uint8_t>& _data, const std::vector<size_t>& _offsets) const
    {
        return reinterpret_cast<T*>(&_data[_offsets[id]]);
    }

    template <typename T>
    T* GetParameter(TIndex index, std::vector<uint8_t>& _data, const std::vector<size_t>& _offsets) const
    {
        auto id = dependencies.parents[index];
        return ObjectFromId<T>(id, _data, _offsets);
    }

    size_t GetReturnValueSize() const override
    {
        if constexpr (has_return)
            return sizeof(TReturn);
        else
            return 0;
    }

    const F functor;
};

} // namespace p7::tasks