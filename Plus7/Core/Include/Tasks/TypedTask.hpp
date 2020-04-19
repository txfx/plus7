#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Task.hpp"
#include "TaskDependencies.hpp"
#include "TaskID.hpp"

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

    using Base     = TaskWithReturn<std::invoke_result_t<F, Ts...>>;
    using TReturn  = typename Base::TReturn;
    using TDepsRef = std::tuple<const TaskWithReturn<Ts>&...>;

    using TIndex = uint16_t;
    template <TIndex... Ids>
    using id_sequence      = std::integer_sequence<TIndex, Ids...>;
    using make_id_sequence = std::make_integer_sequence<TIndex, sizeof...(Ts)>;

    template <std::size_t NParent, std::size_t NChild>
    explicit TypedTask(TypedID<TReturn>                              _id,
                       Name                                          _name,
                       F                                             _functor,
                       TypedTaskDependencies<NParent, NChild, Ts...> _dependencies,
                       const TaskList&                               _tasks)
        : Base(_id, _name, _dependencies)
        , functor(_functor)
        , consumedTasks(GetTaskReferences(_tasks, make_id_sequence {}))
    {}

    template <TIndex... IDs>
    TDepsRef GetTaskReferences(const TaskList& _tasks, id_sequence<IDs...>)
    {
        return { *static_cast<TaskWithReturn<Ts>*>(_tasks[Base::dependencies.parents[IDs]].get())... };
    }

    void Call(std::size_t _index) override
    {
        CallInternal(_index, make_id_sequence {});
    }

    template <TIndex... IDs>
    void CallInternal([[maybe_unused]] std::size_t _index, id_sequence<IDs...>)
    {
        constexpr bool has_arguments = ((!std::is_void_v<Ts> || ...));

        if constexpr (Base::has_return)
        {
            auto& ret = Base::GetReturnValue(_index);
            if constexpr (!has_arguments)
            {
                ret = functor();
            }
            else
            {
                ret = functor(GetParameter<IDs>(_index)...);
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
                functor(GetParameter<IDs>(_index)...);
            }
        }
    }

    template <TIndex ID>
    auto GetParameter(std::size_t _index)
    {
        const auto& task = std::get<ID>(consumedTasks);
        return task.GetReturnValue(_index);
    }

    const F        functor;
    const TDepsRef consumedTasks;
};

} // namespace p7::tasks