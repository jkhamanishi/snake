#include "res/resources.h"
#include "game.h"

using namespace std;

RECT WindowRect()
{
    // RECT of game window in client units (pixels)
    RECT size;
    size.left = 0;
    size.top = 0;
    size.right = GAMEWIDTH;
    size.bottom = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYSIZE) + GAMEHEIGHT;
    return size;
}

LRESULT CALLBACK WindowProcess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps; // paint data for Begin/EndPaint
    static int countdown;
    static GAMESTATE gameState;
    static DIRECTION direction;
    static SNAKEV snake;
    static LOCONGRID foodLoc;
    static RECT snakeRect;
    static ENABLECONTROLS enabledControls;

    auto UpdateGameState = [&](GAMESTATE newState)
    {
        gameState = newState;
    };

    auto InitializeWindow = [&]()
    {
        UpdateGameState(ID_SPLASHSCREEN);
        enabledControls = {
            .wasd = true,
            .arrowkeys = true
        };
    };

    auto ResetGame = [&]()
    {
        countdown = 3;
        UpdateGameState(ID_GAMESTARTING);
        direction = ID_MOVELEFT;
        snake.clear();
        snake.reserve((GAMEWIDTH * GAMEHEIGHT) / (CELLWIDTH * CELLWIDTH));
        snake.push_back({10, 15});
        snake.push_back({11, 15});
        SetTimer(hwnd, ID_COUNTDOWN, COUNTDOWNINTERVAL, NULL);
        SetFoodLoc(&foodLoc, snake);
        RedrawWindow(hwnd, NULL, NULL, (RDW_ERASENOW | RDW_INVALIDATE | RDW_ERASE));
    };

    auto ToggleMenuItem = [&](bool &enableFlag)
    {
        MENUITEMINFO info = {
            .cbSize = sizeof(MENUITEMINFO),
            .fMask = MIIM_STATE,
            .fState = (UINT)((enableFlag = !enableFlag) ? MFS_CHECKED : MFS_UNCHECKED)
        };
        SetMenuItemInfo(GetMenu(hwnd), wParam, FALSE, &info);
        DrawMenuBar(hwnd);
    };

    auto MenuHandler = [&]()
    {
        switch (wParam)
        {
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
        case IDM_PLAYPAUSE:
            switch (gameState)
            {
            case ID_SPLASHSCREEN:
            case ID_GAMEOVER:
                ResetGame();
                break;
            case ID_GAMEPLAY:
            case ID_GAMEPAUSE:
                PlayPause(hwnd, gameState);
                break;
            }
            break;
        case IDM_WASD:
            ToggleMenuItem(enabledControls.wasd);
            break;
        case IDM_ARROWS:
            ToggleMenuItem(enabledControls.arrowkeys);
            break;
        }
    };

    switch (uMsg)
    {
    case WM_CREATE:
        InitializeWindow();
        ShowWindow(hwnd, SW_SHOW);
        break;

    case WM_COMMAND:
        MenuHandler();
        break;

    case WM_TIMER:
        switch (wParam)
        {
        case ID_COUNTDOWN:
            if (!(countdown > 0))
            {
                UpdateGameState(ID_GAMEPLAY);
                KillTimer(hwnd, ID_COUNTDOWN);
                SetTimer(hwnd, ID_GAMETIMER, GAMETIMERINTERVAL, NULL);
            }
            RedrawWindow(hwnd, NULL, NULL, (RDW_ERASENOW | RDW_INVALIDATE | RDW_ERASE));
            break;
        case ID_GAMETIMER:
            UpdateSnake(snake, direction, &foodLoc, snakeRect, gameState);
            RedrawWindow(hwnd, &snakeRect, NULL, (RDW_ERASENOW | RDW_INVALIDATE | RDW_ERASE));
            break;
        case ID_GAMEOVERTIMER:
            KillTimer(hwnd, ID_GAMEOVERTIMER);
            UpdateGameState(ID_NEXTGAMEREADY);
            break;
        }
        break;

    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        switch (gameState)
        {
        case ID_SPLASHSCREEN:
            DisplayTextCenteredMiddle(ps.hdc, "snake game\n\n\npress any button to start");
            break;
        case ID_GAMESTARTING:
            PaintCountdown(ps.hdc, countdown);
            countdown--;
            break;
        case ID_GAMEPLAY:
            PaintGame(ps.hdc, foodLoc, snake);
            break;
        case ID_GAMEPAUSE:
            DisplayTextCenteredMiddle(ps.hdc, "paused");
            break;
        case ID_GAMEOVER:
            PaintGame(ps.hdc, foodLoc, snake);
            PaintGameOverScreen(ps.hdc, snake.size() * 100);
            KillTimer(hwnd, ID_GAMETIMER);
            SetTimer(hwnd, ID_GAMEOVERTIMER, NEXTGAMEWAIT, NULL);
            break;
        }
        EndPaint(hwnd, &ps);
        break;

    case WM_KEYDOWN:
        switch (gameState)
        {
        case ID_SPLASHSCREEN:
        case ID_NEXTGAMEREADY:
            ResetGame();
            break;
        case ID_GAMEPLAY:
            KeyboardHandler(wParam, direction, snake, enabledControls);
            break;
        }
        break;

    case WM_KEYUP:
        if (wParam == VK_ESCAPE)
        {
            PlayPause(hwnd, gameState);
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
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
    RECT gameWinRect = WindowRect();
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