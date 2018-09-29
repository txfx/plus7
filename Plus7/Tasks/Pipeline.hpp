#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "TypedTask.hpp"
#include <Utils/NonCopyable.hpp>

namespace p7::tasks {

struct Pipeline : public NonCopyable
{
    // n parents, n children, receive parents return
    template <typename F, typename... Ts, typename... Us>
    auto CreateTask(F _functor, After<Ts...> _parents, Before<Us...> _children) -> ID<decltype(_functor(std::declval<Ts>()...))>
    {
        static_assert(std::is_invocable<F, Ts...>::value);
        return CreateTask<TypedTask<F, Ts...>>(_functor, _parents.ids, _children.ids);
    }

    // n parents, n children
    template <typename F, typename... Ts, typename... Us>
    auto CreateTask(F _functor, After<Ts...> _parents, Before<Us...> _children) -> ID<decltype(_functor())>
    {
        return CreateTask<TypedTask<F>>(_functor, _parents.ids, _children.ids);
    }

    // n parents, 0 child, receive parents return
    template <typename F, typename... Ts>
    auto CreateTask(F _functor, After<Ts...> _parents) -> ID<decltype(_functor(std::declval<Ts>()...))>
    {
        static_assert(std::is_invocable<F, Ts...>::value);
        return CreateTask<TypedTask<F, Ts...>>(_functor, _parents.ids, NoDependencies{});
    }

    // n parents, 0 child
    template <typename F, typename... Ts>
    auto CreateTask(F _functor, const After<Ts...>& _parents) -> ID<decltype(_functor())>
    {
        return CreateTask<TypedTask<F>>(_functor, _parents.ids, NoDependencies{});
    }

    // 0 parent, n children
    template <typename F, typename... Us>
    auto CreateTask(F _functor, const Before<Us...>& _children) -> ID<decltype(_functor())>
    {
        return CreateTask<TypedTask<F>>(_functor, NoDependencies{}, _children.ids);
    }

    // 0 parent, 0 child
    template <typename F>
    auto CreateTask(F _functor) -> ID<decltype(_functor())>
    {
        return CreateTask<TypedTask<F>>(_functor, NoDependencies{}, NoDependencies{});
    }

protected:
    void ExecuteTasks() const;

private:
    template <std::size_t N>
    using Dependencies   = std::array<Task::InternalId, N>;
    using NoDependencies = Dependencies<0>;

    template <typename TTask, typename F, std::size_t NParents, std::size_t NChildren>
    auto CreateTask(F _functor, const Dependencies<NParents>& _parents, const Dependencies<NChildren>& _children)
    {
        Task::InternalId id = tasks.size();
        tasks.emplace_back(std::make_unique<TTask>(_functor));

        tasks.back()->parents.reserve(tasks.back()->parents.size() + _parents.size());
        for (auto parentId : _parents)
        {
            tasks.back()->parents.push_back(parentId);
            tasks[parentId]->children.push_back(id);
        }

        tasks.back()->children.reserve(tasks.back()->children.size() + _children.size());
        for (auto childId : _children)
        {
            tasks.back()->children.push_back(childId);
            tasks[childId]->parents.push_back(id);
        }

        using R = typename TTask::TReturn;
        if constexpr (!std::is_same<void, R>::value)
        {
            returnValuesSize += sizeof(R);
        }

        return ID<R>(id);
    }

    std::vector<std::unique_ptr<Task>> tasks;

    size_t returnValuesSize = 0;
};

} // namespace p7::tasks