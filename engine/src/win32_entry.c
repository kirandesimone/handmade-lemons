#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    MessageBox(0, "This is a window", "Engine", MB_OK | MB_ICONINFORMATION);
    return 0;
}