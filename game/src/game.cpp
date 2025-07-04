#include "game.hpp"

#include <iostream>

#include <engine/events/window_events.hpp>
#include <engine/graphics/renderer.hpp>
#include <engine/resource_management/resource_manager.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{} // namespace

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
    using namespace game_engine;
    using namespace std::literals;

    m_engine = std::move(engine);

    std::cout << "Game::onInitialize" << std::endl;

    auto rm       = m_engine->GetResourceManager();
    auto renderer = m_engine->GetRenderer();

    m_mesh = rm->LoadMesh("cube"sv,
        {
            .source = "data/3d/cube.obj",
        });

    if (!renderer->Load(m_mesh)) {
        std::cout << "Failed to load mesh" << std::endl;
    }

    m_shader = rm->LoadShader("simple"sv,
        {
            .source_files = {
                             {ShaderType::Vertex, "data/shaders/simple.vert"},
                             {ShaderType::Fragment, "data/shaders/simple.frag"},
                             }
    });

    if (!renderer->Load(m_shader)) {
        std::cout << "Failed to load shader" << std::endl;
    }

    // TODO: do this
    // Создание металлического материала
    //MaterialLoadParams matParams;
    //matParams.shaderName = "PBR";
    //matParams.properties["albedo"] = MaterialProperty{
    //    .type = MaterialProperty::Texture,
    //    .value.texture = resourceManager->GetTexture("Iron_Diffuse")
    //};
    //matParams.properties["metallic"] = MaterialProperty{
    //    .type = MaterialProperty::Float,
    //    .value.floatValue = 0.9f
    //};
    //auto metalMaterial = resourceManager->LoadMaterial("Iron", matParams);
    //
    //// Рендеринг
    //renderer->Render(characterMesh, metalMaterial);

    SubscribeForEvents();

    return true;
}

void Game::Shutdown() noexcept
{
    std::cout << "Game::onShutdown" << std::endl;
    std::cout << " -- updates count: " << m_updates_count << std::endl;
    std::cout << " -- frames count: " << m_frames_count << std::endl;

    UnsubscribeFromEvents();

    m_shader.reset();
    m_mesh.reset();

    m_engine.reset();
}

void Game::OnUpdate(std::chrono::nanoseconds)
{
    m_updates_count++;
}

void Game::OnDraw()
{
    using namespace game_engine;

    m_frames_count++;

    float time = (static_cast<float>(m_frames_count) * 3.14f) / 180.0f;

    const auto model      = glm::rotate(Matrix4(1.0f), time, Vector3(0.5f, 1.0f, 0.0f));
    const auto view       = glm::translate(Matrix4(1.0f), Vector3(0.0f, 0.0f, -3.0f));
    const auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_engine->GetRenderer()->Render(m_mesh,
        m_shader,
        {
            Property{     "model",      model},
            Property{      "view",       view},
            Property{"projection", projection},
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

void Game::SubscribeForEvents()
{
    using namespace game_engine;

    m_subscriptions.push_back(m_engine->GetEventSystem()->Subscribe<KeyboardInputEvent>([this](const auto& event) {
        if (event.key == KeyCode::Escape && event.action == KeyAction::Press) {
            m_engine->SetShouldStopFlag();
        }
    }));
}

void Game::UnsubscribeFromEvents()
{
    m_subscriptions.clear();
}

