#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "Task.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7 {
namespace tasks {

struct Pipeline : public NonCopyable
{
    // n parents, n children, receive parents return
    template <typename F, typename... Ts, typename... Us>
    auto CreateTask(F _functor, After<Ts...> _parents, Before<Us...> _children) -> ID<decltype(_functor(std::declval<Ts>()...))>
    {
        auto task = new TypedTask<
            decltype(_functor(std::declval<Ts>()...)),
            std::tuple<Ts...>,
            make_id_sequence<sizeof...(Ts)>,
            F>(_functor);

        return RegisterTask<decltype(_functor(std::declval<Ts>()...))>(task, _parents.ids, _children.ids);
    }

    // n parents, n children
    template <typename F, typename... Ts, typename... Us>
    auto CreateTask(F _functor, After<Ts...> _parents, Before<Us...> _children) -> ID<decltype(_functor())>
    {
        auto task = new TypedTask<
            decltype(_functor()),
            std::tuple<Ts...>,
            make_id_sequence<sizeof...(Ts)>,
            F>(_functor);

        return RegisterTask<decltype(_functor())>(task, _parents.ids, _children.ids);
    }

    // n parents, 0 child, receive parents return
    template <typename F, typename... Ts>
    auto CreateTask(F _functor, After<Ts...> _parents) -> ID<decltype(_functor(std::declval<Ts>()...))>
    {
        auto task = new TypedTask<
            decltype(_functor(std::declval<Ts>()...)),
            std::tuple<Ts...>,
            make_id_sequence<sizeof...(Ts)>,
            F>(_functor);

        return RegisterTask<decltype(_functor(std::declval<Ts>()...))>(task, _parents.ids, NoDependecies{});
    }

    // n parents, 0 child
    template <typename F, typename... Ts>
    auto CreateTask(F _functor, const After<Ts...>& _parents) -> ID<decltype(_functor())>
    {
        auto task = new TypedTask<
            decltype(_functor()),
            std::tuple<>,
            make_id_sequence<sizeof...(Ts)>,
            F>(_functor);

        return RegisterTask<decltype(_functor())>(task, _parents.ids, NoDependecies{});
    }

    // 0 parent, n children
    template <typename F, typename... Us>
    auto CreateTask(F _functor, const Before<Us...>& _children) -> ID<decltype(_functor())>
    {
        auto task = new TypedTask<
            decltype(_functor()),
            std::tuple<>,
            make_id_sequence<0>,
            F>(_functor);

        return RegisterTask<decltype(_functor())>(task, NoDependecies{}, _children.ids);
    }

    // 0 parent, 0 child
    template <typename F>
    auto CreateTask(F _functor) -> ID<decltype(_functor())>
    {
        auto task = new TypedTask<
            decltype(_functor()),
            std::tuple<>,
            make_id_sequence<0>,
            F>(_functor);

        return RegisterTask<decltype(_functor())>(task, NoDependecies{}, NoDependecies{});
    }

protected:
    void ExecuteTasks() const;

private:
    template <std::size_t N>
    using Dependencies  = std::array<Task::InternalId, N>;
    using NoDependecies = Dependencies<0>;

    template <typename T, std::size_t NParents, std::size_t NChildren>
    ID<T> RegisterTask(Task* _task, const Dependencies<NParents>& _parents, const Dependencies<NChildren>& _children)
    {
        Task::InternalId id = tasks.size();
        tasks.emplace_back(_task);

        _task->parents.reserve(_task->parents.size() + _parents.size());
        for (auto parentId : _parents)
        {
            _task->parents.push_back(parentId);
            tasks[parentId]->children.push_back(id);
        }

        _task->children.reserve(_task->children.size() + _children.size());
        for (auto childId : _children)
        {
            _task->children.push_back(childId);
            tasks[childId]->parents.push_back(id);
        }

        if constexpr (!std::is_same<void, T>::value)
        {
            returnValuesSize += sizeof(T);
        }

        return ID<T>(id);
    }

    std::vector<std::unique_ptr<Task>> tasks;

    size_t returnValuesSize = 0;
};

} // namespace tasks
} // namespace p7