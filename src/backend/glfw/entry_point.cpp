// #include "../core/engine.hpp"
#include "glfw_backend.hpp"

int main() {
    auto backend = std::make_unique<game_engine::backend::GLFWBackend>();
    //game_engine::core::Engine engine(std::move(backend));
    //engine.run();
    return 0;
}
