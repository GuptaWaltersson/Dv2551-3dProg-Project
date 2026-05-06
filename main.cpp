
#include <Windows.h>
#include <iostream>

#include "renderer.hpp"

#define GuptaDebug
#define PontusDebug

#define DebugWindow
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef DebugWindow
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
#endif
    std::cout << "Hello World" << std::endl;
    Renderer renderer;
    renderer.Setup(hInstance, nCmdShow, 1080, 720);
    while (1)
    {

    }
    return 0;
}