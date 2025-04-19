#pragma once

#include <memory>
#include <mutex>

#include <backend.hpp>
#include <glfw/render_thread.hpp>

namespace game_engine::backend
{

class OpenGLShader;
class OpenGLMesh;

class GLFWBackend final : public Backend
{
public:
    GLFWBackend();
    ~GLFWBackend() override;

    // Backend
    bool initialize(const GameSettings& settings) override;
    void shutdown() override;
    void pollEvents() override;
    void beginFrame() override;
    void endFrame() override;

    void applySettings(const GameSettings& settings) override;

    std::shared_ptr<core::Shader> createShader() override;
    std::shared_ptr<core::Mesh> createMesh() override;

    void addRenderCommand(const RenderCommand& command) override;
    void clearRenderCommands() override;
    void executeRenderCommands() override;

    // GLFWBackend
    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleWindowResize(int width, int height);
    void handleWindowMove(int xpos, int ypos);
    void handleWindowClose();
    void handleWindowFocus(bool focused);
    void handleWindowIconify(bool iconified);
    void handleWindowMaximize(bool maximized);

private:

    bool setupOpenGL();
    void applyDisplayMode(const GameSettings& settings);
    void applyAntiAliasing(const GameSettings& settings);

    std::vector<std::shared_ptr<OpenGLShader>> m_shaders;
    std::vector<std::shared_ptr<OpenGLMesh>> m_meshes;

    std::shared_ptr<RenderThread> m_renderThread;

    std::mutex m_commandsMutex;
    std::vector<RenderCommand> m_commands;
};

} // namespace game_engine::backend
