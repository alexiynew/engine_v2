#include <stub_backend.hpp>

namespace game_engine::backend
{

std::shared_ptr<Backend> Backend::Create()
{
    return std::make_shared<StubBackend>();
}

} // namespace game_engine::backend
