#pragma once

#include <memory>

#include "../backend.hpp"
#include <opengl_shader.hpp>

namespace game_engine::backend
{

class GLFWBackend final : public Backend
{
public:
    GLFWBackend(BackendEventHandler& handler);
    ~GLFWBackend();

    bool initialize() override;
    void shutdown() override;
    void poll_events() override;
    void begin_frame() override;
    void end_frame() override;

    void handle_key_event(int key, int scancode, int action, int mods);
    void handle_window_resize(int width, int height);
    void handle_window_move(int xpos, int ypos);
    void handle_window_close();
    void handle_window_focus(bool focused);
    void handle_window_iconify(bool iconified);
    void handle_window_maximize(bool maximized);

private:
    OpenGLShader m_shader;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
};

} // namespace game_engine::backend
