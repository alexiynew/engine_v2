#include "game_stub.hpp"

#include <iostream>

#include <game_engine/core/mesh.hpp>

namespace
{

struct Vertex
{
    game_engine::Vector3 position;
    game_engine::Vector3 normal;
    game_engine::Vector2 uv;
    game_engine::Vector4 color;
};

} // namespace

namespace game_engine::core
{

template <>
inline VertexLayout getVertexLayout<Vertex>()
{
    return {
        .vertexSize = sizeof(Vertex),
        .attributes = {
                       generateAttribute<Vertex, &Vertex::position, offsetof(Vertex, position)>(0, "position"),
                       generateAttribute<Vertex, &Vertex::normal, offsetof(Vertex, normal)>(1, "normal"),
                       generateAttribute<Vertex, &Vertex::uv, offsetof(Vertex, uv)>(2, "uv"),
                       generateAttribute<Vertex, &Vertex::color, offsetof(Vertex, color)>(3, "color"),
                       }
    };
}

} // namespace game_engine::core

namespace game_engine
{
std::shared_ptr<Game> createGameInstance(game_engine::core::Engine& engine)
{
    return std::make_shared<GameStub>(engine);
}

} // namespace game_engine

using namespace game_engine;

GameStub::GameStub(game_engine::core::Engine& engine)
    : m_engine(engine)
{
    std::cout << "GameStub::GameStub" << std::endl;
}

GameStub::~GameStub()
{
    std::cout << "GameStub::~GameStub" << std::endl;
}

void GameStub::onInitialize()
{
    std::cout << "GameStub::onInitialize" << std::endl;

    m_mesh = m_engine.createMesh();

    //// Load a simple mesh (for demonstration purposes)
    //mesh.vertices = {
    //    core::Vertex {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Vertex 1
    //    core::Vertex { {0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Vertex 2
    //    core::Vertex {  {0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}}  // Vertex 3
    //};
    //mesh.subMeshes.push_back({
    //    {0, 1, 2},
    //    {}
    //}); // Single submesh with a triangle

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

    std::vector<core::SubMesh> submeshes{
        {{0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
          12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20},
         {}}
    };

    m_mesh->setMeshData(createMeshData(vertices, submeshes, core::PrimitiveType::Triangles));

    m_mesh->flush();
}

void GameStub::onUpdate(std::chrono::nanoseconds)
{
    m_updatesCount++;
}

void GameStub::onDraw()
{
    m_framesCount++;

    m_engine.render(m_mesh);
}

void GameStub::onShutdown()
{
    std::cout << "GameStub::onShutdown" << std::endl;
    std::cout << " -- updates count: " << m_updatesCount << std::endl;
    std::cout << " -- frames count: " << m_framesCount << std::endl;
}

void GameStub::onKeyboardInputEvent(const KeyboardInputEvent& event)
{
    if (event.key == KeyCode::Escape && event.action == KeyAction::Press) {
        m_engine.setShouldStopFlag();
    }
}

bool GameStub::onShouldClose()
{
    return true;
}

GameSettings GameStub::getSettings()
{
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
