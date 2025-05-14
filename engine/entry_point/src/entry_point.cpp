#ifdef USE_WINMAIN_ENTRY
    #include <windows.h>
#endif

#include <engine/game.hpp>

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>

#include <engine_impl.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

#ifdef USE_WINMAIN_ENTRY
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif

{
    using namespace game_engine;

    try {
        const ServiceLocator locator = []() {
            ServiceLocator sl;

            sl.registerFactory<backend::Backend>();
            sl.registerFactory<graphics::Renderer>();

            return sl;
        }();

        EngineImpl engine(locator);

        auto game = CreateGameInstance(engine);
        engine.setGameInstance(std::move(game));
        auto returnCode = engine.run();

        return returnCode;

    } catch (std::exception& e) {
        LOG_ERROR << e.what() << std::endl;
        return -1;
    } catch (...) {
        LOG_ERROR << "Unknown exception" << std::endl;
        return -1;
    }
}
