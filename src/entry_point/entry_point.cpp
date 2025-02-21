#include <backend/glfw/glfw_backend.hpp>
#include <core/engine_impl.hpp>

int main() {
    auto backend = std::make_shared<game_engine::backend::GLFWBackend>();
    auto engine  = std::make_shared<game_engine::core::EngineImpl>(backend);

    const int returnCode = engine->run();

    return returnCode;
}
