#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <vector>
#include "res/resources.h"
#include "game.h"
#include <cstdio> // for printf()

using namespace std;

LRESULT CALLBACK WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // HDC hdc;                    // handle to device context (DC)
    PAINTSTRUCT ps;       // paint data for Begin/EndPaint
    static HDC hdcCompat; // handle to DC for bitmap
    // static POINT pt;            // x- and y-coordinates of cursor

    static int gameState;
    static int direction;
    static vector<LOCONGRID> snake;
    static LOCONGRID foodLoc;

    auto SetInitialValues = [&]()
    {
        gameState = -2;
        direction = 1;
    };

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
        InvalidateRgn(hwnd, NULL, FALSE);
        SetInitialValues();
        snake.push_back({10, 10});
        snake.push_back({10, 11});
        snake.push_back({11, 11});
        snake.push_back({12, 11});
        snake.push_back({12, 12});
        snake.push_back({12, 13});
        snake.push_back({12, 14});
        snake.push_back({11, 14});
        snake.push_back({11, 13});
        snake.push_back({11, 12});
        SetFoodLoc(&foodLoc, snake);
        break;

    case WM_COMMAND:
        MenuHandler();
        break;

    case WM_PAINT:
        // DisplayTextCenteredMiddle(hwnd, L"snake game");

        BeginPaint(hwnd, &ps);
        GamePainter(ps.hdc, foodLoc, snake);
        EndPaint(hwnd, &ps);
        break;

    case WM_KEYDOWN:
        KeyboardHandler(wParam, &direction, snake);
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
    HWND hwnd = CreateWindow(
        CLASS_NAME, // Window class
        L"Snake",   // Window title
        winStyle,   // Window style

        CW_USEDEFAULT,      // Window X position
        CW_USEDEFAULT,      // Windox Y position
        gameWinRect.right,  // Window width in device units
        gameWinRect.bottom, // Window height in device units

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