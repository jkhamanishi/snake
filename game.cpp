#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "game.h"

RECT GameRect()
{
    // RECT of game window (not including title and menu) in client coordinates
    RECT size;
    size.left = 0;
    size.top = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYSIZE);
    size.right = GAMEWIDTH;
    size.bottom = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYSIZE) + GAMEHEIGHT;
    return size;
}

void DisplayTextCenteredMiddle(HWND hwnd, LPCTSTR message)
{
    RECT rect = GameRect();
    HDC wdc = GetWindowDC(hwnd);
    SetTextColor(wdc, RGB(255, 255, 255));
    SetBkMode(wdc, OPAQUE);
    SetBkColor(wdc, RGB(0, 0, 0));
    DrawText(wdc, message, -1, &rect, (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
    DeleteDC(wdc);
}

void DisplayCenteredText(HWND hwnd, LPCTSTR message, int distFromTop)
{
    RECT rect = GameRect();
    rect.top += distFromTop;
    HDC wdc = GetWindowDC(hwnd);
    SetTextColor(wdc, RGB(255, 255, 255));
    SetBkMode(wdc, TRANSPARENT);
    DrawText(wdc, message, -1, &rect, (DT_SINGLELINE | DT_NOCLIP));
    DeleteDC(wdc);
}

void KeyboardHandler(WPARAM key, int& direction)
{
    switch (key)
    {
    case 0x41:    // A key
    case VK_LEFT: // LEFT ARROW key
        direction = ID_MOVELEFT;
        return;
    case 0x53:    // S key
    case VK_DOWN: // DOWN ARROW key
        direction = ID_MOVEDOWN;
        return;
    case 0x44:     // D key
    case VK_RIGHT: // RIGHT ARROW key
        direction = ID_MOVERIGHT;
        return;
    case 0x57:  // W
    case VK_UP: // UP ARROW key
        direction = ID_MOVEUP;
        return;
    default:
        return;
    }
}