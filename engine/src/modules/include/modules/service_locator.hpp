#pragma once

#include <any>
#include <concepts>
#include <functional>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <typeindex>
#include <unordered_map>

namespace game_engine
{

template <typename T, typename... Args>
concept ModuleInterfaceWithArgs = requires(Args&&... args) {
    {
        T::Create(std::forward<Args>(args)...)
    } -> std::same_as<std::shared_ptr<T>>;
};

template <typename T>
concept ModuleInterface = requires {
    {
        T::Create
    } -> std::convertible_to<decltype(&T::Create)>;
};

class ServiceLocator final
{
public:
    ServiceLocator()  = default;
    ~ServiceLocator() = default;

    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator(ServiceLocator&&)      = default;

    ServiceLocator& operator=(const ServiceLocator&) = delete;
    ServiceLocator& operator=(ServiceLocator&&)      = default;

    template <ModuleInterface Interface>
    void registerModule()
    {
        const std::type_index key{typeid(Interface)};
        if (m_creators.contains(key)) {
            throw std::runtime_error("Module already registered: " + std::string(key.name()));
        }

        using ArgsTuple      = typename InterfaceTraits<Interface>::ArgsTuple;
        using CreatorWrapper = typename InterfaceTraits<Interface>::CreatorWrapper;

        CreatorWrapper wrapper = [](ArgsTuple&& argsTuple) {
            return std::apply(Interface::Create, std::move(argsTuple));
        };

        m_creators[key] = std::move(wrapper);
    }

    template <ModuleInterface Interface, typename... Args>
    requires ModuleInterfaceWithArgs<Interface, Args...>
    std::shared_ptr<Interface> resolve(Args&&... args) const
    {
        const std::type_index key{typeid(Interface)};

        auto it = m_creators.find(key);
        if (it == m_creators.end()) {
            throw std::runtime_error("Module not registered: " + std::string(key.name()));
        }

        using CreatorWrapper = typename InterfaceTraits<Interface>::CreatorWrapper;

        const CreatorWrapper* creator = std::any_cast<CreatorWrapper>(&it->second);
        if (!creator) {
            throw std::runtime_error("Arguments types mismatch for " + std::string(key.name()));
        }

        return (*creator)(std::forward_as_tuple(args...));
    }

private:
    template <typename T>
    struct MethodTraits;

    template <typename R, typename... Args>
    struct MethodTraits<R (*)(Args...)>
    {
        using ArgsTuple = std::tuple<Args...>;
    };

    template <typename T>
    struct InterfaceTraits
    {
        using MethodPtr      = decltype(&T::Create);
        using ArgsTuple      = typename MethodTraits<MethodPtr>::ArgsTuple;
        using CreatorWrapper = std::function<std::shared_ptr<T>(ArgsTuple&&)>;
    };

    std::unordered_map<std::type_index, std::any> m_creators;
};

} // namespace game_engine
