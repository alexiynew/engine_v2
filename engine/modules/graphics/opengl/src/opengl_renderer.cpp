#include "opengl_renderer.hpp"

#include <glad/glad.h>
#include <opengl_mesh.hpp>
#include <opengl_shader.hpp>
#include <opengl_texture.hpp>

namespace game_engine::graphics
{

OpenGLRenderer::OpenGLRenderer()
{}

OpenGLRenderer::~OpenGLRenderer()
{
    Shutdown();
}

#pragma region Renderer

bool OpenGLRenderer::Init()
{
    if (gladLoadGL() == 0 || GLVersion.major != 3 || GLVersion.minor != 3) {
        throw std::runtime_error("Unsupported OpenGL version");
    }

    glEnable(GL_DEPTH_TEST);
    // TODO: enable antialiasing from settings
    glEnable(GL_MULTISAMPLE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    return true;
}

void OpenGLRenderer::Shutdown() noexcept
{
    m_meshes.clear();
    m_shaders.clear();
}

bool OpenGLRenderer::Load(const std::shared_ptr<IMesh>& mesh)
{
    OpenGLMesh opengl_mesh;
    if (!opengl_mesh.Load(mesh)) {
        throw std::runtime_error("Mesh loading failed");
    }

    const auto [_, inserted] = m_meshes.emplace(mesh->GetId(), std::move(opengl_mesh));

    return inserted;
}

bool OpenGLRenderer::Load(const std::shared_ptr<IShader>& shader)
{
    OpenGLShader opengl_shader;
    if (!opengl_shader.Load(shader)) {
        throw std::runtime_error("Shader loading failed");
    }

    const auto [_, inserted] = m_shaders.emplace(shader->GetId(), std::move(opengl_shader));

    return inserted;
}

bool OpenGLRenderer::Load(const std::shared_ptr<ITexture>& texture)
{
    OpenGLTexture opengl_texture;
    if (!opengl_texture.Load(texture)) {
        throw std::runtime_error("Texture loading failed");
    }

    const auto& [_, inserted] = m_textures.emplace(texture->GetId(), std::move(opengl_texture));

    return inserted;
}

void OpenGLRenderer::Execute(const BeginFrameCommand& command)
{
    glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Execute(const EndFrameCommand& command)
{}

void OpenGLRenderer::Execute(const RenderCommand& command)
{
    const auto shader_it = m_shaders.find(command.shader);
    if (shader_it == m_shaders.end()) {
        throw std::runtime_error("Shader not found");
    }

    const auto& shader = shader_it->second;

    if (!shader.IsValid()) {
        throw std::runtime_error("Shader not valid");
    }

    const auto mesh_it = m_meshes.find(command.mesh);
    if (mesh_it == m_meshes.end()) {
        throw std::runtime_error("Mesh not found");
    }

    const auto& mesh = mesh_it->second;

    if (!mesh.IsValid()) {
        throw std::runtime_error("Mesh not valid");
    }

    shader.Use();

    for (const auto& property : command.properties) {
        shader.SetProperty(property);
    }

    if (command.instance_count > 1) {
        // TODO: Implement instancing
    } else {
        mesh.Render();
    }
}

#pragma endregion

} // namespace game_engine::graphics

