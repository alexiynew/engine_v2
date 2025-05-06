#include "game.hpp"

#include <iostream>

#include <game_engine/core/mesh.hpp>
#include <game_engine/core/shader.hpp>

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

inline game_engine::core::VertexLayout getVertexLayout()
{
    return {
        .vertexSize = sizeof(Vertex),
        .attributes = {
                       game_engine::core::generateAttribute(0, "position", &Vertex::position),
                       game_engine::core::generateAttribute(1, "normal", &Vertex::normal),
                       game_engine::core::generateAttribute(2, "uv", &Vertex::uv),
                       game_engine::core::generateAttribute(3, "color", &Vertex::color),
                       }
    };
}

} // namespace

Game::Game(game_engine::core::Engine& engine)
    : m_engine(engine)
{
    std::cout << "Game::Game" << std::endl;
}

Game::~Game()
{
    std::cout << "Game::~Game" << std::endl;
}

void Game::onInitialize()
{
    using namespace game_engine::core;

    std::cout << "Game::onInitialize" << std::endl;

    const std::vector<SubMesh> submeshes = {
        {cube_mesh::submesh_indices, {}}
    };

    m_mesh = m_engine.createMesh();
    m_mesh->setMeshData(createMeshData(cube_mesh::vertices, submeshes, PrimitiveType::Triangles, getVertexLayout()));
    m_mesh->flush();

    m_shader = m_engine.createShader();
    m_shader->setSource(VertexShaderSource, FragmentShaderSource);
    if (!m_shader->link()) {
        std::cout << "Failed to load shader" << std::endl;
    }
}

void Game::onUpdate(std::chrono::nanoseconds)
{
    m_updatesCount++;
}

void Game::onDraw()
{
    m_framesCount++;

    float time = (static_cast<float>(m_framesCount) * 3.14f) / 180.0f;

    using game_engine::Matrix4;
    using game_engine::Vector3;
    using game_engine::core::Uniform;

    const auto model      = glm::rotate(Matrix4(1.0f), time, Vector3(0.5f, 1.0f, 0.0f));
    const auto view       = glm::translate(Matrix4(1.0f), Vector3(0.0f, 0.0f, -3.0f));
    const auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_engine.render(m_mesh,
                    m_shader,
                    {
                        Uniform{     "model",      model},
                        Uniform{      "view",       view},
                        Uniform{"projection", projection},
    });
}

void Game::onShutdown()
{
    std::cout << "Game::onShutdown" << std::endl;
    std::cout << " -- updates count: " << m_updatesCount << std::endl;
    std::cout << " -- frames count: " << m_framesCount << std::endl;

    m_shader.reset();
    m_mesh.reset();
}

void Game::onKeyboardInputEvent(const game_engine::KeyboardInputEvent& event)
{
    using namespace game_engine;

    if (event.key == KeyCode::Escape && event.action == KeyAction::Press) {
        m_engine.setShouldStopFlag();
    }
}

bool Game::onShouldClose()
{
    return true;
}

game_engine::GameSettings Game::getSettings()
{
    using namespace game_engine;

    GameSettings settings;
    settings.resolutionWidth  = 1920;
    settings.resolutionHeight = 1080;
    settings.displayMode      = DisplayMode::Windowed;
    settings.frameRate        = 60;
    settings.updateRate       = 120;
    settings.antiAliasing     = AntiAliasing::MSAA4x;
    settings.vSync            = true;

    return settings;
}
