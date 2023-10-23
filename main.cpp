#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "res/resources.h"
#include "game.h"
#include <string>
#include <cstdio> // for printf()

LRESULT CALLBACK WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // HDC hdc;                    // handle to device context (DC)
    PAINTSTRUCT ps;       // paint data for Begin/EndPaint
    static HDC hdcCompat; // handle to DC for bitmap
    // static POINT pt;            // x- and y-coordinates of cursor

    static int gameState;
    static int direction;

    auto MenuHandler = [&]()
    {
        switch (wParam)
        {
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
        case IDM_PLAYPAUSE:
            gameState = (gameState > 0) ? 0 : 1;
            break;
        }
    };

    switch (uMsg)
    {
    case WM_CREATE:
        ShowWindow(hwnd, SW_SHOW);
        gameState = -2;
        direction = 1;
        break;

    case WM_COMMAND:
        MenuHandler();
        break;

    case WM_PAINT:
        // DisplayTextCenteredMiddle(hwnd, L"snake game");
        DisplayTextCenteredMiddle(hwnd, LPCTSTR(std::to_string(direction).c_str()));

        BeginPaint(hwnd, &ps);
        GamePainter(hwnd, ps.hdc);
        EndPaint(hwnd, &ps);
        break;

    case WM_KEYDOWN:
        KeyboardHandler(wParam, &direction);
        InvalidateRgn(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        // Destroy the background brush, compatible bitmap, and bitmap.
        DeleteDC(hdcCompat);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Hide the external console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Game Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcess;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_SNAKEICON));
    RegisterClass(&wc);

    // Window Style
    const DWORD winStyle = (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    // Get desired window size in device units
    RECT gameWinRect = GameRect();
    AdjustWindowRect(&gameWinRect, (winStyle | ~WS_OVERLAPPED), TRUE); // For some reason the WS_OVERLAPPED style is not allowed.

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                           // Optional window styles.
        CLASS_NAME,                  // Window class
        L"Snake in C++ Using Win32", // Window text
        winStyle,                    // Window style

        CW_USEDEFAULT,      // Window X position
        CW_USEDEFAULT,      // Windox Y position
        gameWinRect.right,  // Window height in device units
        gameWinRect.bottom, // Window width in device units

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );
    if (hwnd == NULL)
    {
        return 0;
    }

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}