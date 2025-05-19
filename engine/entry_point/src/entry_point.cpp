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
        auto createModuleLocator = []() {
            ModuleLocator ml;

            ml.setImplementation(backend::IBackend::Create());
            ml.setImplementation(graphics::IRenderer::Create());
            ml.setImplementation(IGame::Create());

            return ml;
        };

        auto engine = std::make_shared<EngineImpl>(createModuleLocator());
        return engine->run();

    } catch (std::exception& e) {
        LOG_ERROR << e.what() << std::endl;
        return -1;
    } catch (...) {
        LOG_ERROR << "Unknown exception" << std::endl;
        return -1;
    }
}
