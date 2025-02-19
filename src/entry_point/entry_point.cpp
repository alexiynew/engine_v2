#include <core/engine_impl.hpp>

int main() {
    auto engine = std::make_shared<game_engine::core::EngineImpl>();

    const int returnCode = engine->run();

    return returnCode;
}
