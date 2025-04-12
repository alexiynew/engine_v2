#pragma once

#include <expected>
#include <memory>
#include <mutex>

#include <backend.hpp>

namespace game_engine::backend
{

class OpenGLShader;
class OpenGLMesh;

class GLFWBackend final : public Backend
{
public:
    GLFWBackend();
    ~GLFWBackend() override;

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

    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleWindowResize(int width, int height);
    void handleWindowMove(int xpos, int ypos);
    void handleWindowClose();
    void handleWindowFocus(bool focused);
    void handleWindowIconify(bool iconified);
    void handleWindowMaximize(bool maximized);

private:

    std::vector<std::shared_ptr<OpenGLShader>> m_shaders;
    std::vector<std::shared_ptr<OpenGLMesh>> m_meshes;

    std::vector<RenderCommand> m_renderCommands;
    std::mutex m_renderCommandsMutex;

    bool setupOpenGL();
    void applyDisplayMode(const GameSettings& settings);
    void applyAntiAliasing(const GameSettings& settings);
};

} // namespace game_engine::backend
