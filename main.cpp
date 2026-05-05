
#include <Windows.h>
#include <iostream>

#define GuptaDebug
#define PontusDebug

//#define DebugWindow
int main()
{
#ifdef DebugWindow
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
#endif
    std::cout << "Hello World" << std::endl;

    return 0;
}