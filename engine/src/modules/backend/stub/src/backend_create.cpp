#include <stub_backend.hpp>

namespace game_engine::backend
{

std::shared_ptr<IBackend> IBackend::Create()
{
    return std::make_shared<StubBackend>();
}

} // namespace game_engine::backend
