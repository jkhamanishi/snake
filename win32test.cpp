#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <cstdio>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles.
        CLASS_NAME,          // Window class
        L"snake",            // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, // Window X position
        CW_USEDEFAULT, // Windox Y position
        500,           // Window width
        500,           // Window height

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );
    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
