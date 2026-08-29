#ifndef UNICODE
#define UNICODE
#endif

// Prevent Legacy Win32 Stuff
#define WIN32_LEAN_AND_MEAN

// Standard Header Files
#include <windows.h>
#include <shellapi.h>
#include <dwmapi.h>

// Custom Header Files
#include "element.h"

// Linker Add
#pragma comment(lib, "dwmapi.lib")

// DWM Backdrop Attributes
#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

#ifndef DWMSBT_TRANSIENTBACKDROP
#define DWMSBT_TRANSIENTBACKDROP 3
#endif

// Elements
const RECT SCREEN = set_screen_bounds();
const RECT TERMINAL = set_terminal_bounds(SCREEN);

// Function Prototypes
LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK terminal_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Main Function
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Window
    WNDCLASS window_class = { };
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = L"MainWindowClass";
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    
    HWND hwnd_window = CreateWindowEx(
        WS_EX_LAYERED,                              // optional window styles
        L"Blank Window Class",                      // window class name
        L"Window",                                  // window title
        WS_POPUP | WS_VISIBLE,                      // window style
        SCREEN.left,                                // x
        SCREEN.top,                                 // y
        SCREEN.right - SCREEN.left,                 // width
        SCREEN.bottom - SCREEN.top,                 // height
        NULL,                                       // parent window    
        NULL,                                       // menu
        hInstance,                                  // instance handle
        NULL                                        // additiona app data
    );

    if (hwnd_window == NULL) return 0;

    // Background Tranparent Effect
    SetLayeredWindowAttributes(hwnd_window, 0, 200, LWA_ALPHA);

    // Terminal Window
    WNDCLASS terminal_class = { };
    terminal_class.lpfnWndProc = terminal_proc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = L"MainWindowClass";
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    HWND hwnd_terminal = CreateWindowEx(
        WS_EX_LAYERED,                              // optional window styles
        CLASS_NAME,                                 // window class name
        L"Terminal",                                // window title
        WS_POPUP | WS_VISIBLE,                      // window style
        SCREEN.left,                                // x
        SCREEN.top,                                 // y
        SCREEN.right - SCREEN.left,                 // width
        SCREEN.bottom - SCREEN.top,                 // height
        NULL,                                       // parent window    
        NULL,                                       // menu
        hInstance,                                  // instance handle
        NULL                                        // additiona app data
    );

    if (hwnd_terminal == NULL) return 0;

    // Display All Windows
    ShowWindow(hwnd, nCmdShow);

    UpdateWindow(hwnd);

    // Message Loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Function Definitions ----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Enable Blur Effect For Full Window
            int backdropType = DWMSBT_TRANSIENTBACKDROP;
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));

            // Restrict Blur Effect For Terminal Only
            HRGN hRgn = CreateRectRgn(TERMINAL.left, TERMINAL.top, TERMINAL.right, TERMINAL.bottom);
            SetWindowRgn(hwnd, hRgn, TRUE);

            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Set Drawing Elements
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

            // Terminal Border
            Rectangle(hdc, TERMINAL.left, TERMINAL.top, TERMINAL.right, TERMINAL.bottom);

            // Clean Drawing Elements
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0); // terminate message loop
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // default message handling
    }
}