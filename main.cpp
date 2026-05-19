
#include <Windows.h>
#include <iostream>
#include <chrono>
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


    auto previousFrame = std::chrono::high_resolution_clock::now();
    MSG msg = {};
    while (msg.message != WM_QUIT && !(GetKeyState(VK_ESCAPE) & 0x8000))
    {
        auto currentFrame = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> frametime = currentFrame - previousFrame;
        previousFrame = currentFrame;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        renderer.Movement(frametime.count());
        renderer.renderFrame((float)frametime.count());
        //std::cout << frametime.count() << std::endl;
    }
    return 0;
}