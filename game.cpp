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

void KeyboardHandler(WPARAM key, int *direction)
{
    switch (key)
    {
    case 0x41: // A key
    case 0x25: // LEFT ARROW key
        *direction = ID_MOVELEFT;
        return;
    case 0x53: // S key
    case 0x28: // DOWN ARROW key
        *direction = ID_MOVEDOWN;
        return;
    case 0x44: // D key
    case 0x27: // RIGHT ARROW key
        *direction = ID_MOVERIGHT;
        return;
    case 0x57: // W key
    case 0x26: // UP ARROW key
        *direction = ID_MOVEUP;
        return;
    default:
        return;
    }
}

void GamePainter(HWND hwnd, HDC hdc)
{
    // Select the brush
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SelectObject(hdc, GetStockObject(DC_PEN));

    // Paint the snake
    SetDCBrushColor(hdc, RGB(0, 130, 0));
    SetDCPenColor(hdc, RGB(0, 0, 255));
    // Rectangle(hdc, 50, 50, 70, 70);
    for (int i = 0; i < GAMEWIDTH / CELLWIDTH; ++i)
    {
        for (int j = 0; j < GAMEHEIGHT / CELLWIDTH; ++j)
        {
            int left = i*CELLWIDTH;
            int top = j*CELLWIDTH;
            int right = (i+1)*CELLWIDTH;
            int bottom = (j+1)*CELLWIDTH;
            Rectangle(hdc, left, top, right, bottom);
        }
    }
}