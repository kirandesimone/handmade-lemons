#include <Windows.h>


LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (message) {
        case WM_SIZE: {
            OutputDebugStringA("Resize\n");
            break;
        }

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("Active\n");
            break;
        }

        case WM_DESTROY: {
            OutputDebugStringA("Destroyed\n");
            break;
        }

        case WM_CLOSE: {
            OutputDebugStringA("Close\n");
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC displayContext = BeginPaint(window, &paint); // PAINT begins
            // All painting occurs here
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            PatBlt(displayContext, x, y, width, height, WHITENESS);

            EndPaint(window, &paint); // PAINT ends
            break;
        }

        default: {
            OutputDebugStringA("Default\n");
            result = DefWindowProcW(window, message, wParam, lParam);
            break;
        }
    }

    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    WNDCLASS windowClass = {};

    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = L"HandmadeLemonsWindow";

    if (RegisterClass(&windowClass)) {

        HWND windowHandle = CreateWindowEx(
            0,
            windowClass.lpszClassName,
            L"Handmade Lemons",
            WS_OVERLAPPED | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0);

        if (windowHandle) {
            MSG message = {};
            while (GetMessage(&message, 0, 0, 0)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            } 
        }
    } else {
        OutputDebugStringA("Cant register class\n");
    }
    return 0;
}

