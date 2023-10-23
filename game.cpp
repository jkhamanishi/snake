#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "game.h"
#include <cstdio> // for printf()

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

void SetLocation(LOCONGRID *loc, int x, int y)
{
    (*loc).x = x;
    (*loc).y = y;
}

LOCINPIXELS GridToPixel(LOCONGRID loc)
{
    LOCINPIXELS locPxl;
    locPxl.x = CELLHALFWIDTH + loc.x * CELLWIDTH;
    locPxl.y = CELLHALFWIDTH + loc.y * CELLWIDTH;
    return locPxl;
}

void SetFoodLoc(LOCONGRID* foodLoc, LOCONGRID snake[])
{
    int n = 2;
    (*foodLoc).x = rand() % (GAMEWIDTH / CELLWIDTH);
    (*foodLoc).y = rand() % (GAMEHEIGHT / CELLWIDTH);
    for (int i = 0; i < n; i++)
    {
        if (snake[i].x == (*foodLoc).x && snake[i].y == (*foodLoc).y )
        {
            SetFoodLoc(foodLoc, snake);
            break;
        }
    }
}

void PaintGrid(HDC hdc)
{
    // Select and set the pan and brush colors
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, RGB(20, 20, 20));
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // Paint the grid
    for (int i = 0; i < GAMEWIDTH / CELLWIDTH; ++i)
    {
        for (int j = 0; j < GAMEHEIGHT / CELLWIDTH; ++j)
        {
            int left = i * CELLWIDTH;
            int top = j * CELLWIDTH;
            int right = (i + 1) * CELLWIDTH;
            int bottom = (j + 1) * CELLWIDTH;
            Rectangle(hdc, left, top, right, bottom);
        }
    }
}

void PaintFood(HDC hdc, LOCONGRID loc)
{
    // Select and set the pan and brush colors
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(255, 0, 0));

    LOCINPIXELS locPxl = GridToPixel(loc);
    int left = locPxl.x - CONTENTRADIUS;
    int top = locPxl.y - CONTENTRADIUS;
    int right = locPxl.x + CONTENTRADIUS;
    int bottom = locPxl.y + CONTENTRADIUS;

    Ellipse(hdc, left, top, right, bottom);
}

void GamePainter(HDC hdc, LOCONGRID foodLoc, LOCONGRID snake[])
{
    PaintGrid(hdc);
    PaintFood(hdc, foodLoc);
}