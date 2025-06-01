#pragma once

#include <memory>
#include <string_view>

#include <engine/graphics/material.hpp>
#include <engine/graphics/mesh.hpp>
#include <engine/graphics/shader.hpp>
#include <engine/graphics/texture.hpp>
#include <engine/resource.hpp>

namespace game_engine
{

class IResourceManager
{
public:

    virtual ~IResourceManager() = default;

    virtual std::shared_ptr<IMesh> LoadMesh(const std::string_view name, const MeshLoadParams& params)             = 0;
    virtual std::shared_ptr<IShader> LoadShader(const std::string_view name, const ShaderLoadParams& params)       = 0;
    virtual std::shared_ptr<ITexture> LoadTexture(const std::string_view name, const TextureLoadParams& params)    = 0;
    virtual std::shared_ptr<IMaterial> LoadMaterial(const std::string_view name, const MaterialLoadParams& params) = 0;

    virtual std::shared_ptr<IMesh> GetMesh(const std::string_view name) const         = 0;
    virtual std::shared_ptr<IShader> GetShader(const std::string_view name) const     = 0;
    virtual std::shared_ptr<ITexture> GetTexture(const std::string_view name) const   = 0;
    virtual std::shared_ptr<IMaterial> GetMaterial(const std::string_view name) const = 0;

    virtual bool IsLoaded(const std::string_view name) const = 0;
    virtual void Unload(const std::string_view name)         = 0;
    virtual void UnloadAll()                                 = 0;
};

} // namespace game_engine
