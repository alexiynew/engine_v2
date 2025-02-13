#include <backend/backend.hpp>
#include <core/engine_impl.hpp>

int main() {
    auto backend = game_engine::backend::create_backend_instance();
    auto engine  = std::make_shared<game_engine::core::EngineImpl>(backend);

    backend->set_event_handler(engine);

    const int return_code = engine->run();

    return return_code;
}
