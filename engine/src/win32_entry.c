#include <Windows.h>

#define global_var static

global_var int isRunning;
global_var BITMAPINFO bitMapInfo;
global_var void *bitMapMemory;

static void ResizeDIBSection(int width, int height) {
    // Build buffer using WindowsAPI so we can draw (backbuffer)
    // What is a bitmap?

    if (bitMapMemory) {
        VirtualFree(bitMapMemory, 0, MEM_RELEASE);
    }

    bitMapInfo.bmiHeader.biSize = sizeof(bitMapInfo.bmiHeader);
    bitMapInfo.bmiHeader.biWidth = width;
    bitMapInfo.bmiHeader.biHeight = height;
    bitMapInfo.bmiHeader.biPlanes = 1;
    bitMapInfo.bmiHeader.biBitCount = 32;
    bitMapInfo.bmiHeader.biCompression = BI_RGB; // No Compression

    int pixelByteSize = 4;
    int bitMapMemorySize = (width * height) * pixelByteSize;
    bitMapMemory = VirtualAlloc(0, bitMapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

static void RefreshWindow(HDC deviceContext, int x, int y, int width, int height) {
    StretchDIBits(
        deviceContext,
        x, y, width, height, // Destination
        x, y, width, height, // Source
        bitMapMemory,
        &bitMapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (message) {
        case WM_SIZE: {
            OutputDebugStringA("Resize\n");
            RECT clientRect;
            GetClientRect(window, &clientRect);
            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;
            ResizeDIBSection(width, height);
            break;
        }

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("Active\n");
            break;
        }

        case WM_CLOSE: {
            OutputDebugStringA("Close\n");
            isRunning = FALSE;
            break;
        }

        case WM_DESTROY: {
            OutputDebugStringA("Destroyed\n");
            isRunning = FALSE;
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint); // PAINT begins
            // All painting occurs here
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            RefreshWindow(deviceContext, x, y, width, height);
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
    WNDCLASS windowClass = {0};

    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = L"HandmadeLemonsWindow";

    if (RegisterClass(&windowClass)) {

        HWND windowHandle = CreateWindowExA(
            0,
            windowClass.lpszClassName,
            L"Handmade Lemons",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0);

        if (windowHandle) {
            isRunning = TRUE;
            while(isRunning){
                MSG message = {0};
                BOOL messageResult = GetMessageA(&message, 0, 0, 0);
                if (messageResult > 0) {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                } else {
                    break;
                }
            } 
        }
    } else {
        OutputDebugStringA("Cant register class\n");
    }

    return 0;
}

