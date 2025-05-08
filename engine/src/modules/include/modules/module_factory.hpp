#pragma once

#include <functional>
#include <memory>
#include <stdexcept>

namespace game_engine
{

template <typename Interface>
class ModuleFactory
{
public:
    using PtrType = std::shared_ptr<Interface>;

    // Must be defined in module
    static void RegisterModule();

    template <typename... Args>
    static PtrType Create(Args&&... args)
    {
        auto& creator = CreatorStorage<Args...>();
        if (!creator) {
            throw std::runtime_error("Module not registered");
        }
        return creator(std::forward<Args>(args)...);
    }

private:
    template <typename T>
    struct FunctionTraits;

    template <typename ClassType, typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType (ClassType::*)(Args...) const>
    {
        using Return    = ReturnType;
        using ArgsTuple = std::tuple<Args...>;
    };

    template <typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType (*)(Args...)>
    {
        using Return    = ReturnType;
        using ArgsTuple = std::tuple<Args...>;
    };

    template <typename Function>
    static void RegisterCreator(Function&& creator)
    {
        using Traits     = FunctionTraits<decltype(&std::decay_t<Function>::operator())>;
        using ReturnType = typename Traits::Return;
        using ArgsTuple  = typename Traits::ArgsTuple;

        static_assert(std::is_convertible_v<ReturnType, PtrType>, "Creator must return std::shared_ptr<Interface>.");

        RegisterImpl(std::forward<Function>(creator), ArgsTuple{});
    }

    template <typename... Args>
    using Creator = std::function<PtrType(Args...)>;

    template <typename Function, typename... Args>
    static void RegisterImpl(Function&& creator, std::tuple<Args...>)
    {
        Creator<Args...> wrapper  = creator;
        CreatorStorage<Args...>() = wrapper;
    }

    template <typename... Args>
    static auto& CreatorStorage()
    {
        static Creator<Args...> s_creator;
        return s_creator;
    }
};

}; // namespace game_engine
