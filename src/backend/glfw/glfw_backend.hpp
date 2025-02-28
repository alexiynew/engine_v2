#pragma once

#include <expected>
#include <memory>

#include <backend.hpp>

namespace game_engine::backend
{

class OpenGLShader;

class GLFWBackend final : public Backend
{
public:
    GLFWBackend();
    ~GLFWBackend() override;

    bool initialize() override;
    void shutdown() override;
    void pollEvents() override;
    void beginFrame() override;
    void endFrame() override;

    core::MeshId loadMesh(const core::Mesh& mesh) override;
    void renderMesh(core::MeshId meshId) override;

    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleWindowResize(int width, int height);
    void handleWindowMove(int xpos, int ypos);
    void handleWindowClose();
    void handleWindowFocus(bool focused);
    void handleWindowIconify(bool iconified);
    void handleWindowMaximize(bool maximized);

private:

    struct MeshInfo
    {
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;

        std::size_t indicesCount = 0;
    };

    std::unique_ptr<OpenGLShader> m_shader;

    core::MeshId m_nextMeshId = 0; ///< ID counter for loaded meshes
    std::unordered_map<std::size_t, MeshInfo> m_loadedMeshes;

    std::expected<MeshInfo, bool> loadMeshToGPU(const core::Mesh& mesh);
};

} // namespace game_engine::backend
