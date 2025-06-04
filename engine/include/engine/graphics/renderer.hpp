#pragma once

#include <memory>
#include <vector>

#include <engine/graphics/material.hpp>
#include <engine/graphics/mesh.hpp>
#include <engine/graphics/property.hpp>
#include <engine/graphics/shader.hpp>

namespace game_engine
{

class IRenderer
{
public:

    virtual ~IRenderer() = default;

    virtual bool Load(const std::shared_ptr<IMesh>& mesh)       = 0;
    virtual bool Load(const std::shared_ptr<IShader>& shader)   = 0;
    virtual bool Load(const std::shared_ptr<ITexture>& texture) = 0;

    virtual void Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IShader>& shader, std::vector<Property> properties) = 0;
    virtual void Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material)                               = 0;

    virtual void EndFrame() = 0;
};

} // namespace game_engine
