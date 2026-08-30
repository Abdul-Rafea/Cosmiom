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
LRESULT CALLBACK main_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK terminal_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Main Function
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Main Window Class
    HBRUSH main_brush = CreateSolidBrush(RGB(0, 0, 0));

    WNDCLASS window_class = { };
    window_class.lpfnWndProc = main_proc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = L"MainWindowClass";
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = main_brush;
    
    if (!RegisterClass(&window_class)) return 0;

    // Terminal Window Class
    HBRUSH terminal_brush = CreateSolidBrush(RGB(255, 255, 255));

    WNDCLASS terminal_class = { };
    terminal_class.lpfnWndProc = terminal_proc;
    terminal_class.hInstance = hInstance;
    terminal_class.lpszClassName = L"TerminalWindowClass";
    terminal_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    terminal_class.hbrBackground = terminal_brush;

    if (!RegisterClass(&terminal_class)) return 0;

    // Main Windwo Instance
    HWND hwnd_main = CreateWindowEx(
        WS_EX_LAYERED,                              // optional window styles
        L"MainWindowClass",                         // window class name
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

    if (hwnd_main == NULL) return 0;

    // Terminal Window Instance
    HWND hwnd_terminal = CreateWindowEx(
        WS_EX_LAYERED,                              // optional window styles
        L"TerminalWindowClass",                     // window class name
        NULL,                                       // window title
        WS_POPUP | WS_VISIBLE,                      // window style
        TERMINAL.left,                              // x
        TERMINAL.top,                               // y
        TERMINAL.right - TERMINAL.left,             // width
        TERMINAL.bottom - TERMINAL.top,             // height
        hwnd_main,                                  // parent window    
        NULL,                                       // menu
        hInstance,                                  // instance handle
        NULL                                        // additiona app data
    );

    if (hwnd_terminal == NULL) return 0;

    // Background Tranparent Effect
    SetLayeredWindowAttributes(hwnd_main, 0, 125, LWA_ALPHA);
    SetLayeredWindowAttributes(hwnd_terminal, 0, 175, LWA_ALPHA);

    // Display All Windows
    ShowWindow(hwnd_main, nCmdShow);
    UpdateWindow(hwnd_main);
    ShowWindow(hwnd_terminal, nCmdShow);
    UpdateWindow(hwnd_terminal);

    // Message Loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Function Definitions ----------------------------------------------------------------------------
LRESULT CALLBACK main_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK terminal_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}