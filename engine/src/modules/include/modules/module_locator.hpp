#pragma once

#include <concepts>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace game_engine
{

template <typename T>
concept ModuleInterface = requires {
    {
        T::Create()
    } -> std::same_as<std::shared_ptr<T>>;
};

class ModuleLocator final
{
public:
    ModuleLocator()  = default;
    ~ModuleLocator() = default;

    ModuleLocator(const ModuleLocator&) = delete;
    ModuleLocator(ModuleLocator&&)      = default;

    ModuleLocator& operator=(const ModuleLocator&) = delete;
    ModuleLocator& operator=(ModuleLocator&&)      = default;

    template <ModuleInterface Interface>
    void setImplementation(std::shared_ptr<Interface>&& module)
    {
        const std::type_index key{typeid(Interface)};
        if (m_modules.contains(key)) {
            throw std::runtime_error("Module already registered: " + std::string(key.name()));
        }

        m_modules[key] = std::move(module);
    }

    template <ModuleInterface Interface>
    std::shared_ptr<Interface> get() const
    {
        const std::type_index key{typeid(Interface)};

        auto it = m_modules.find(key);
        if (it == m_modules.end()) {
            throw std::runtime_error("Module not registered: " + std::string(key.name()));
        }

        return std::static_pointer_cast<Interface>(it->second);
    }

private:

    std::unordered_map<std::type_index, std::shared_ptr<void>> m_modules;
};

} // namespace game_engine
