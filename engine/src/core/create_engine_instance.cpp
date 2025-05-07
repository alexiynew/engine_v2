#include <game_engine/core/engine.hpp>

#include <core/engine_impl.hpp>

namespace game_engine::core
{
std::shared_ptr<Engine> createEngineInstance()
{
    return std::make_shared<EngineImpl>();
}
} // namespace game_engine::core
