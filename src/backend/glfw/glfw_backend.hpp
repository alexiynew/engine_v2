#pragma once

#include <memory>

#include "../backend.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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

private:
    GLFWwindow* m_window = nullptr;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
    static void window_close_callback(GLFWwindow* window);
    static void window_focus_callback(GLFWwindow* window, int focused);
    static void window_iconify_callback(GLFWwindow* window, int iconified);
    static void window_maximize_callback(GLFWwindow* window, int maximized);
};

} // namespace game_engine::backend
