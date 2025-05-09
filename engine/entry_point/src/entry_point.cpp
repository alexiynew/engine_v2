#ifdef USE_WINMAIN_ENTRY
    #include <windows.h>
#endif

#include <engine/engine.hpp>

#ifdef USE_WINMAIN_ENTRY
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif

{
    return game_engine::createEngineInstance()->run();
}
