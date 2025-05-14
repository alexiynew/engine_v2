#pragma once

#include <any>
#include <functional>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <typeindex>
#include <unordered_map>

namespace game_engine
{

class ModuleFactory final
{
public:
    ModuleFactory() = default;

    ModuleFactory(const ModuleFactory&) = delete;
    ModuleFactory(ModuleFactory&&)      = default;

    ModuleFactory& operator=(const ModuleFactory&) = delete;
    ModuleFactory& operator=(ModuleFactory&&)      = default;

    template <typename Interface>
    void set(std::shared_ptr<Interface>&& instance)
    {
        const auto key = std::type_index(typeid(Interface));

        if (m_instances.count(key) > 0) {
            throw std::runtime_error("Instance for " + std::string(typeid(Interface).name()) + " already registered");
        }

        m_instances[key] = std::move(instance);
    }

    template <typename Interface>
    std::shared_ptr<Interface> get() const
    {
        const auto key = std::type_index(typeid(Interface));

        auto it = m_instances.find(key);
        if (it == m_instances.end()) {
            throw std::runtime_error("Instance not registered: " + std::string(typeid(Interface).name()));
        }

        return std::static_pointer_cast<Interface>(it->second);
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_instances;
};

} // namespace game_engine
