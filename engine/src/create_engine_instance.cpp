#include <engine/engine.hpp>

#include <engine_impl.hpp>

namespace game_engine
{
std::shared_ptr<Engine> createEngineInstance()
{
    return std::make_shared<EngineImpl>();
}
} // namespace game_engine
