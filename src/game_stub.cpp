#include "game_stub.hpp"

#include <iostream>

#include <game_engine/core/mesh.hpp>
#include <game_engine/core/logger.hpp>

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
    game_engine::core::Logger() << "GameStub::GameStub";
}

GameStub::~GameStub()
{
    game_engine::core::Logger() << "GameStub::~GameStub";
}

void GameStub::onInitialize()
{
    game_engine::core::Logger() << "GameStub::onInitialize";

    core::Mesh mesh;

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

    mesh.vertices = {
        // Front edge (red)
        core::Vertex{ {-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Vertex 0
        core::Vertex{  {0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Vertex 1
        core::Vertex{   {0.5f, 0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, // Vertex 2
        core::Vertex{  {-0.5f, 0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, // Vertex 3

        //Back edge (green)
        core::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Vertex 4
        core::Vertex{ {0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Vertex 5
        core::Vertex{  {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, // Vertex 6
        core::Vertex{ {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, // Vertex 7

        // Upper edge (blue)
        core::Vertex{  {-0.5f, 0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Vertex 8
        core::Vertex{   {0.5f, 0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Vertex 9
        core::Vertex{  {0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // Vertex 10
        core::Vertex{ {-0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // Vertex 11

        // Bottom edge (yellow)
        core::Vertex{ {-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}, // Vertex 12
        core::Vertex{  {0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}, // Vertex 13
        core::Vertex{ {0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}, // Vertex 14
        core::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}, // Vertex 15

        // Left edge (blue)
        core::Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}}, // Vertex 16
        core::Vertex{ {-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}}, // Vertex 17
        core::Vertex{  {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}, // Vertex 18
        core::Vertex{ {-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}, // Vertex 19

        // Right edge (purple)
        core::Vertex{ {0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}}, // Vertex 20
        core::Vertex{  {0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}}, // Vertex 21
        core::Vertex{   {0.5f, 0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}}, // Vertex 22
        core::Vertex{  {0.5f, 0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}}  // Vertex 23
    };

    mesh.subMeshes.push_back({
        {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
         12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20},
        {}
    });

    mesh.primitiveType = core::PrimitiveType::Triangles;
    mesh.calculateBoundingBox();

    m_meshId = m_engine.loadMesh(std::move(mesh));
}

void GameStub::onUpdate(std::chrono::nanoseconds)
{
    m_updatesCount++;
}

void GameStub::onDraw()
{
    m_framesCount++;

    m_engine.renderMesh(m_meshId);
}

void GameStub::onShutdown()
{
    game_engine::core::Logger() << "GameStub::onShutdown";
    game_engine::core::Logger() << " -- updates count: " << m_updatesCount;
    game_engine::core::Logger() << " -- frames count: " << m_framesCount;
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
