#ifdef USE_WINMAIN
    #include <windows.h>
#endif

#include <core/engine_impl.hpp>
#include <modules/backend.hpp>
#include <modules/module_factory.hpp>
#include <modules/renderer.hpp>

#ifdef USE_WINMAIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif

{
    using namespace game_engine;

    ModuleFactory<Backend>::RegisterModule();
    ModuleFactory<Renderer>::RegisterModule();

    auto engineInstance = []() {
        auto backend  = ModuleFactory<Backend>::Create();
        auto renderer = ModuleFactory<Renderer>::Create(backend->getRenderContext());
        auto engine   = std::make_shared<game_engine::core::EngineImpl>(backend, renderer);
        auto game     = createGameInstance(*engine);

        engine->setGameInstance(game);

        return engine;
    }();

    const int returnCode = engineInstance->run();

    return returnCode;
}
