#ifdef USE_WINMAIN
    #include <windows.h>
#endif

#include <game_engine/core/engine.hpp>

#ifdef USE_WINMAIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif

{
    return game_engine::core::createEngineInstance()->run();
}
