#include "game_stub.hpp"

#include <iostream>

namespace game_engine
{
std::unique_ptr<Game> create_game_instance(game_engine::core::Engine& engine) {
    return std::make_unique<GameStub>(engine);
}
} // namespace game_engine

using namespace game_engine;

GameStub::GameStub(game_engine::core::Engine& engine)
    : m_engine(engine) {
    std::cout << "GameStub::GameStub" << std::endl;
}

GameStub::~GameStub() {
    std::cout << "GameStub::~GameStub" << std::endl;
}

void GameStub::on_initialize() {
    std::cout << "GameStub::on_initialize" << std::endl;
}

void GameStub::on_update(std::chrono::nanoseconds elapsed_time) {
    m_updates_count++;
}

void GameStub::on_draw() {
    m_frames_count++;
}

void GameStub::on_shutdown() {
    std::cout << "GameStub::on_shutdown" << std::endl;
    std::cout << " -- updates count: " << m_updates_count << std::endl;
    std::cout << " -- frames count: " << m_frames_count << std::endl;
}

void GameStub::on_keyboard_input_event(const KeyboardInputEvent& event) {
    if (event.key == KeyCode::Escape && event.action == KeyAction::Press) {
        m_engine.set_should_stop_flag();
    }
}

bool GameStub::on_should_close() {
    return true;
}
