#include "game.hpp"

#include <iostream>

#include <engine/event_system.hpp>
#include <engine/graphics/mesh.hpp>
#include <engine/graphics/shader.hpp>
#include <engine/window_events.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{

struct Vertex
{
    game_engine::Vector3 position;
    game_engine::Vector3 normal;
    game_engine::Vector2 uv;
    game_engine::Vector4 color;
};

namespace triangle_mesh
{

std::vector<Vertex> vertices = {
    Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 1
    Vertex{ {0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 2
    Vertex{  {0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}  // Vertex 3
};

std::vector<unsigned int> submesh_indices = {0, 1, 2};

} // namespace triangle_mesh

namespace cube_mesh
{

std::vector<Vertex> vertices = {
    // Front edge (red)
    Vertex{ {-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 0
    Vertex{  {0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 1
    Vertex{   {0.5f, 0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 2
    Vertex{  {-0.5f, 0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 3

    //Back edge (green)
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 4
    Vertex{ {0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 5
    Vertex{  {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 6
    Vertex{ {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 7

    // Upper edge (blue)
    Vertex{  {-0.5f, 0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 8
    Vertex{   {0.5f, 0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 9
    Vertex{  {0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 10
    Vertex{ {-0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 11

    // Bottom edge (yellow)
    Vertex{ {-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 12
    Vertex{  {0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 13
    Vertex{ {0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 14
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 15

    // Left edge (blue)
    Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 16
    Vertex{ {-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 17
    Vertex{  {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 18
    Vertex{ {-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 19

    // Right edge (purple)
    Vertex{ {0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 20
    Vertex{  {0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 21
    Vertex{   {0.5f, 0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 22
    Vertex{  {0.5f, 0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 23
};

std::vector<unsigned int> submesh_indices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                             12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

} // namespace cube_mesh

const char* VertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aUV;
        layout(location = 3) in vec4 aColor;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec4 color;
        void main() {
            color = aColor;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

const char* FragmentShaderSource = R"(
        #version 330 core
        in vec4 color;

        out vec4 FragColor;

        void main() {
            FragColor = color;
            //FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

inline game_engine::graphics::VertexLayout getVertexLayout()
{
    return {
        .vertex_size = sizeof(Vertex),
        .attributes  = {
                        game_engine::graphics::GenerateAttribute(0, "position", &Vertex::position),
                        game_engine::graphics::GenerateAttribute(1, "normal", &Vertex::normal),
                        game_engine::graphics::GenerateAttribute(2, "uv", &Vertex::uv),
                        game_engine::graphics::GenerateAttribute(3, "color", &Vertex::color),
                        }
    };
}

} // namespace

Game::Game()
{
    std::cout << "Game::Game" << std::endl;
}

Game::~Game()
{
    std::cout << "Game::~Game" << std::endl;
}

bool Game::Init(std::shared_ptr<game_engine::IEngine> engine) noexcept
{
    m_engine = std::move(engine);

    using namespace game_engine::graphics;

    std::cout << "Game::onInitialize" << std::endl;

    const std::vector<SubMesh> submeshes = {
        {cube_mesh::submesh_indices, {}}
    };

    m_mesh = m_engine->CreateMesh();
    m_mesh->SetMeshData(CreateMeshData(cube_mesh::vertices, submeshes, PrimitiveType::Triangles, getVertexLayout()));
    m_mesh->Flush();

    m_shader = m_engine->CreateShader();
    m_shader->SetSource(VertexShaderSource, FragmentShaderSource);
    if (!m_shader->Link()) {
        std::cout << "Failed to load shader" << std::endl;
    }

    subscribeForEvents();

    return true;
}

void Game::Shutdown() noexcept
{
    std::cout << "Game::onShutdown" << std::endl;
    std::cout << " -- updates count: " << m_updatesCount << std::endl;
    std::cout << " -- frames count: " << m_framesCount << std::endl;

    unsubscribeFromEvents();

    m_shader.reset();
    m_mesh.reset();

    m_engine.reset();
}

void Game::OnUpdate(std::chrono::nanoseconds)
{
    m_updatesCount++;
}

void Game::OnDraw()
{
    m_framesCount++;

    float time = (static_cast<float>(m_framesCount) * 3.14f) / 180.0f;

    using game_engine::Matrix4;
    using game_engine::Vector3;
    using game_engine::graphics::Uniform;

    const auto model      = glm::rotate(Matrix4(1.0f), time, Vector3(0.5f, 1.0f, 0.0f));
    const auto view       = glm::translate(Matrix4(1.0f), Vector3(0.0f, 0.0f, -3.0f));
    const auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_engine->Render(m_mesh,
                     m_shader,
                     {
                         Uniform{     "model",      model},
                         Uniform{      "view",       view},
                         Uniform{"projection", projection},
    });
}

bool Game::OnShouldClose()
{
    return true;
}

game_engine::GameSettings Game::GetSettings()
{
    using namespace game_engine;

    GameSettings settings;
    settings.resolution_width  = 1920;
    settings.resolution_height = 1080;
    settings.display_mode      = DisplayMode::Windowed;
    settings.frame_rate        = 60;
    settings.update_rate       = 120;
    settings.anti_aliasing     = AntiAliasing::MSAA4x;
    settings.v_sync            = true;

    return settings;
}

void Game::subscribeForEvents()
{
    using namespace game_engine;

    m_subscriptions.push_back(m_engine->GetEventSystem().Subscribe<KeyboardInputEvent>([this](const auto& event) {
        if (event.key == KeyCode::Escape && event.action == KeyAction::Press) {
            m_engine->SetShouldStopFlag();
        }
    }));
}

void Game::unsubscribeFromEvents()
{
    m_subscriptions.clear();
}

