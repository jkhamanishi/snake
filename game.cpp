#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "game.h"
#include <vector>
#include <cstdio> // for printf()

using namespace std;

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

int NextBodySegmentDirection(vector<LOCONGRID> snake, int currentSegmentIndex = 0)
{
    int nextSegmentIndex = currentSegmentIndex + 1;
    LOCONGRID currentSegment = snake.at(currentSegmentIndex);
    LOCONGRID nextSegment = snake.at(nextSegmentIndex);
    int diffX = nextSegment.x - currentSegment.x;
    int diffY = nextSegment.y - currentSegment.y;
    return ((diffX << 1) | diffY);
}

void KeyboardHandler(WPARAM key, int *direction, vector<LOCONGRID> snake)
{
    int invalidDirection = NextBodySegmentDirection(snake);
    switch (key)
    {
    case 0x41: // A key
    case 0x25: // LEFT ARROW key
        *direction = (invalidDirection != ID_MOVELEFT) ? ID_MOVELEFT : *direction;
        return;
    case 0x53: // S key
    case 0x28: // DOWN ARROW key
        *direction = (invalidDirection != ID_MOVEDOWN) ? ID_MOVEDOWN : *direction;
        return;
    case 0x44: // D key
    case 0x27: // RIGHT ARROW key
        *direction = (invalidDirection != ID_MOVERIGHT) ? ID_MOVERIGHT : *direction;
        return;
    case 0x57: // W key
    case 0x26: // UP ARROW key
        *direction = (invalidDirection != ID_MOVEUP) ? ID_MOVEUP : *direction;
        return;
    default:
        return;
    }
}

LOCINPIXELS GridToPixel(LOCONGRID loc)
{
    LOCINPIXELS locPxl;
    locPxl.x = CELLHALFWIDTH + loc.x * CELLWIDTH;
    locPxl.y = CELLHALFWIDTH + loc.y * CELLWIDTH;
    return locPxl;
}

void SetFoodLoc(LOCONGRID *foodLoc, vector<LOCONGRID> snake)
{
    (*foodLoc).x = rand() % (GAMEWIDTH / CELLWIDTH);
    (*foodLoc).y = rand() % (GAMEHEIGHT / CELLWIDTH);
    for (LOCONGRID bodySegment : snake)
    {
        if (bodySegment.x == (*foodLoc).x && bodySegment.y == (*foodLoc).y)
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
    for (int i = 0; i < GAMEWIDTH / CELLWIDTH; i++)
    {
        for (int j = 0; j < GAMEHEIGHT / CELLWIDTH; j++)
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

void PaintSnake(HDC hdc, vector<LOCONGRID> snake)
{
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(0, 130, 0));
    for (int i = 0; i < snake.size()-1; i++)
    {
        LOCINPIXELS currentLocPxl = GridToPixel(snake.at(i));
        LOCINPIXELS nextLocPxl = GridToPixel(snake.at(i+1));
        int direction = NextBodySegmentDirection(snake, i);
        int left, top, right, bottom;
        switch (direction)
        {
        case ID_MOVEUP:
        case ID_MOVELEFT:
            left = nextLocPxl.x - CONTENTRADIUS;
            top = nextLocPxl.y - CONTENTRADIUS;
            right = currentLocPxl.x + CONTENTRADIUS;
            bottom = currentLocPxl.y + CONTENTRADIUS;
            break;
        case ID_MOVEDOWN:
        case ID_MOVERIGHT:
            left = currentLocPxl.x - CONTENTRADIUS;
            top = currentLocPxl.y - CONTENTRADIUS;
            right = nextLocPxl.x + CONTENTRADIUS;
            bottom = nextLocPxl.y + CONTENTRADIUS;
            break;
        default:
            return;
        }
        Rectangle(hdc, left, top, right, bottom);
    }
}

void GamePainter(HDC hdc, LOCONGRID foodLoc, vector<LOCONGRID> snake)
{
    PaintGrid(hdc);
    PaintSnake(hdc, snake);
    PaintFood(hdc, foodLoc);
}