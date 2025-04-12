#include "backend.hpp"

namespace game_engine::backend
{

void Backend::attachBackendObserver(BackendObserver& observer)
{
    m_observers.push_front(observer);
}

void Backend::detachBackendObserver(const BackendObserver& observer)
{
    m_observers.remove_if([&observer](const RefObserver& obj) { return &obj.get() == &observer; });
}

} // namespace game_engine::backend
