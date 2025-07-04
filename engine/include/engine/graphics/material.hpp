#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <engine/graphics/property.hpp>
#include <engine/resource_management/resource.hpp>

namespace game_engine
{

struct MaterialLoadParams
{
    std::filesystem::path source;
};

class IMaterial : public IResource
{
public:

    ~IMaterial() override = default;

    virtual void SetProperty(const std::string& name, const PropertyValue& property)    = 0;
    virtual const std::unordered_map<std::string, PropertyValue>& GetProperties() const = 0;
};

} // namespace game_engine
