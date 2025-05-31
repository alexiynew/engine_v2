#pragma once

#include <filesystem>

#include <engine/graphics/property.hpp>
#include <engine/resource.hpp>

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

    virtual void SetProperty(const std::string& name, const PropertyValue& property) = 0;
};

} // namespace game_engine
