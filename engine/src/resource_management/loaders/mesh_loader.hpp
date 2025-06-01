#pragma once

#include <memory>

#include <engine/graphics/mesh.hpp>

namespace game_engine
{

class MeshLoader final
{
public:

    std::shared_ptr<IMesh> Load(const MeshLoadParams& params) const;
};

} // namespace game_engine
