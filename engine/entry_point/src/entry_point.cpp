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
        const ModuleFactory factory = []() {
            ModuleFactory mf;

            mf.set<backend::Backend>(ModuleRegistrar<backend::Backend>::Create());

            auto backend = mf.get<backend::Backend>();
            mf.set<graphics::Renderer>(ModuleRegistrar<graphics::Renderer>::Create(backend->getRenderContext()));

            return mf;
        }();

        EngineImpl engine(factory);

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
