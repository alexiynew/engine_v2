#pragma once

#include <memory>

#include "../backend.hpp"

namespace game_engine::backend
{

class GLFWBackend : public Backend
{
public:
    GLFWBackend();
    ~GLFWBackend();

    bool initialize() override;
    void shutdown() override;
    void poll_events() override;
    void begin_frame() override;
    void end_frame() override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace game_engine::backend
