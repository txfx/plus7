#pragma once

#include "Task.hpp"
#include "TaskDependencies.hpp"
#include "TaskID.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace p7::tasks {

template <typename T>
struct TaskWithReturn : Task
{
protected:
    using Task::Task;
    using TReturn = T;

    static constexpr bool has_return = !(std::is_void_v<TReturn>);

    T& GetReturnValue(std::size_t index) { return returnValues[index % N]; }

public:
    const T& GetReturnValue(std::size_t index) const { return returnValues[index % N]; }

private:
    static constexpr std::size_t N = std::is_void_v<T> ? 0 : 2;

    std::array<T, N> returnValues;
};

template <>
struct TaskWithReturn<void> : Task
{
protected:
    using Task::Task;
    using TReturn = void;

    static constexpr bool has_return = false;
};

template <typename F, typename... Ts>
struct TypedTask final : TaskWithReturn<std::invoke_result_t<F, Ts...>>
{
private:
    friend struct Pipeline;

    using Base    = TaskWithReturn<std::invoke_result_t<F, Ts...>>;
    using TReturn = typename Base::TReturn;

    template <typename T>
    struct TDepRef
    {
        const TaskWithReturn<T>& task;
        const int32_t            offset;
    };
    using TDepsRef = std::tuple<TDepRef<Ts>...>;

    using TIndex = uint16_t;
    template <TIndex... Ids>
    using id_sequence      = std::integer_sequence<TIndex, Ids...>;
    using make_id_sequence = std::make_integer_sequence<TIndex, sizeof...(Ts)>;

    template <std::size_t NParent, std::size_t NChild>
    explicit TypedTask(
      TypedID<TReturn>                              _id,
      Name                                          _name,
      F                                             _functor,
      TypedTaskDependencies<NParent, NChild, Ts...> _dependencies,
      const TaskList&                               _tasks)
        : Base(_id, _name, _dependencies)
        , functor(_functor)
        , dependencies(GetDependencies(_tasks, make_id_sequence {}))
    { }

    template <typename T>
    TDepRef<T> GetTask(std::size_t _index, const TaskList& _tasks) const
    {
        const ID    parent = Base::dependencies.parents[_index];
        const Task* task   = (parent == self<void>) ? this : _tasks[parent].get();
        return { static_cast<const TaskWithReturn<T>&>(*task), parent.GetOffset() };
    }

    template <TIndex... IDs>
    TDepsRef GetDependencies(const TaskList& _tasks, id_sequence<IDs...>) const
    {
        return { GetTask<Ts>(IDs, _tasks)... };
    }

    void Call(std::size_t _index) override { CallInternal(_index, make_id_sequence {}); }

    template <TIndex... IDs>
    void CallInternal([[maybe_unused]] std::size_t _index, id_sequence<IDs...>)
    {
        if constexpr (Base::has_return)
        {
            auto& ret = Base::GetReturnValue(_index);
            ret.~TReturn();
            new (&ret) TReturn { functor(GetParameter<IDs>(_index)...) };
        }
        else
        {
            functor(GetParameter<IDs>(_index)...);
        }
    }

    template <TIndex ID>
    auto GetParameter(std::size_t _index)
    {
        const auto& dependency = std::get<ID>(dependencies);
        return dependency.task.GetReturnValue(_index + dependency.offset);
    }

    const F        functor;
    const TDepsRef dependencies;
};

} // namespace p7::tasks