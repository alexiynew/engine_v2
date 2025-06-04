#ifdef USE_WINMAIN_ENTRY
    #include <windows.h>
#endif

#include <engine/game.hpp>

#include <modules/backend/backend_module.hpp>
#include <modules/graphics/renderer_module.hpp>

#include <engine_impl.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

#ifdef USE_WINMAIN_ENTRY
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
#else
int main(int argc, char* argv[])
#endif

{
    using namespace game_engine;

    try {
        auto create_module_locator = []() {
            ModuleLocator ml;

            ml.SetImplementation(backend::IBackendModule::Create());
            ml.SetImplementation(graphics::IRendererModule::Create());
            ml.SetImplementation(IGame::Create());

            return ml;
        };

        auto engine = std::make_shared<EngineImpl>(create_module_locator());
        return engine->Run();

    } catch (std::exception& e) {
        LOG_ERROR << e.what() << std::endl;
        return -1;
    } catch (...) {
        LOG_ERROR << "Unknown exception" << std::endl;
        return -1;
    }
}
