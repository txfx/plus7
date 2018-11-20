#pragma once

#include <Utils/NonCopyable.hpp>

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

namespace p7 {

struct App;
struct ModuleRepository;

struct Module : public NonCopyable
{
public:
    explicit Module(App& _app)
        : app(_app)
    {}
    virtual ~Module() = default;

protected:
    App& app;
};

// ===========================================================================

template <typename... Ts>
struct ModuleWithDependencies : public Module
{
public:
    explicit constexpr ModuleWithDependencies(App& _app);

protected:
    template <typename T>
    constexpr T& Get() const { return std::get<T&>(dependencies); }

private:
    template <typename T>
    static T& GetDependency(ModuleRepository& _repository, App& _app);

private:
    const std::tuple<Ts&...> dependencies;
};

// ===========================================================================

struct ModuleRepository
{
protected:
    template <typename T, typename... TArgs>
    T& LoadModule(TArgs&&... args);

private:
    template <typename... Ts>
    friend struct ModuleWithDependencies;

    template <typename T>
    T* GetDependency() const;

private:
    std::vector<std::unique_ptr<Module>> modules;
};

// ===========================================================================

template <typename... Ts>
constexpr ModuleWithDependencies<Ts...>::ModuleWithDependencies(App& _app)
    : Module(_app)
    , dependencies(GetDependency<Ts>(_app, _app)...)
{}

template <typename... Ts>
template <typename T>
T& ModuleWithDependencies<Ts...>::GetDependency(ModuleRepository& _repository, App& _app)
{
    auto module = _repository.GetDependency<T>();
    return module != nullptr ? *module : _repository.LoadModule<T>(_app);
}

// ===========================================================================

template <typename T, typename... TArgs>
T& ModuleRepository::LoadModule(TArgs&&... args)
{
    modules.emplace_back(std::make_unique<T>(args...));
    return dynamic_cast<T&>(*modules.back());
}

template <typename T>
T* ModuleRepository::GetDependency() const
{
    auto it = std::find_if(modules.begin(), modules.end(), [](const auto& mod) {
        return dynamic_cast<T*>(mod.get()) != nullptr;
    });

    if (it != modules.end())
    {
        return dynamic_cast<T*>(it->get());
    }
    else
    {
        return nullptr;
    }
}

} // namespace p7