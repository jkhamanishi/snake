#include "game.h"
#include <ctime>  // for rand()
#include <cstdio> // for printf() and sprintf()

void PauseGame(HWND hwnd, GAMESTATE &gameState)
{
    if (gameState >= ID_GAMEPAUSE)
    {
        gameState = ID_GAMEPAUSE;
        KillTimer(hwnd, ID_GAMETIMER);
        RedrawWindow(hwnd, NULL, NULL, (RDW_ERASENOW | RDW_INVALIDATE | RDW_ERASE));
    }
}
void UnpauseGame(HWND hwnd, GAMESTATE &gameState)
{
    gameState = ID_GAMEPLAY;
    SetTimer(hwnd, ID_GAMETIMER, GAMETIMERINTERVAL, NULL);
    RedrawWindow(hwnd, NULL, NULL, (RDW_ERASENOW | RDW_INVALIDATE | RDW_ERASE));
}
void PlayPause(HWND hwnd, GAMESTATE &gameState)
{
    switch (gameState)
    {
    case ID_GAMEPLAY:
        PauseGame(hwnd, gameState);
        break;
    case ID_GAMEPAUSE:
        UnpauseGame(hwnd, gameState);
        break;
    }
}

LPCTSTR charToLPCTSTR(const char *text)
{
    const size_t size = strlen(text) + 1;
    wchar_t *wText = new wchar_t[size];
    mbstowcs(wText, text, size);
    return (LPCTSTR)wText;
}

void DisplayTextCenteredMiddle(HDC hdc, const char *message)
{
    DisplayCenteredText(hdc, message, (GAMEHEIGHT / 2));
}

void DisplayCenteredText(HDC hdc, const char *message, int distFromTop)
{
    LPCTSTR lpString = charToLPCTSTR(message);

    RECT rect, textRect = {0, 0, 0, 0};
    GetClientRect(WindowFromDC(hdc), &rect);
    DrawText(hdc, lpString, -1, &textRect, DT_CALCRECT);
    rect.top += distFromTop - (textRect.bottom / 2);

    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(0, 0, 0));
    DrawText(hdc, lpString, -1, &rect, (DT_CENTER | DT_NOCLIP));
}

DIRECTION NextBodySegmentDirection(SNAKEV snake, int currentSegmentIndex = 0)
{
    LOCONGRID currentSegment = snake.at(currentSegmentIndex);
    LOCONGRID nextSegment = snake.at(currentSegmentIndex + 1);
    int diffX = nextSegment.x - currentSegment.x;
    int diffY = nextSegment.y - currentSegment.y;
    return ((diffX << 1) | diffY);
}

void KeyboardHandler(WPARAM key, DIRECTION &direction, SNAKEV snake, ENABLECONTROLS enabledControls)
{
    auto ChangeDirection = [&](bool enabled, DIRECTION newDirection)
    {
        direction = (enabled && newDirection != NextBodySegmentDirection(snake)) ? newDirection : direction;
    };
    switch (key)
    {
    case 'W':
        return ChangeDirection(enabledControls.wasd, ID_MOVEUP);
    case 'A': // A key
        return ChangeDirection(enabledControls.wasd, ID_MOVELEFT);
    case 'S': // S key
        return ChangeDirection(enabledControls.wasd, ID_MOVEDOWN);
    case 'D': // D key
        return ChangeDirection(enabledControls.wasd, ID_MOVERIGHT);
    case VK_LEFT: // LEFT ARROW key
        return ChangeDirection(enabledControls.arrowkeys, ID_MOVELEFT);
    case VK_DOWN: // DOWN ARROW key
        return ChangeDirection(enabledControls.arrowkeys, ID_MOVEDOWN);
    case VK_RIGHT: // RIGHT ARROW key
        return ChangeDirection(enabledControls.arrowkeys, ID_MOVERIGHT);
    case VK_UP: // UP ARROW key
        return ChangeDirection(enabledControls.arrowkeys, ID_MOVEUP);
    }
}

void InvalidateSnakeRegion(RECT &snakeRect, SNAKEV snake)
{
    snakeRect.left = snake.at(0).x;
    snakeRect.top = snake.at(0).y;
    snakeRect.right = snake.at(0).x;
    snakeRect.bottom = snake.at(0).y;
    for (int i = 1; i < snake.size(); i++)
    {
        snakeRect.left = (snake.at(i).x < snakeRect.left) ? snake.at(i).x : snakeRect.left;
        snakeRect.top = (snake.at(i).y < snakeRect.top) ? snake.at(i).y : snakeRect.top;
        snakeRect.right = (snake.at(i).x > snakeRect.right) ? snake.at(i).x : snakeRect.right;
        snakeRect.bottom = (snake.at(i).y > snakeRect.bottom) ? snake.at(i).y : snakeRect.bottom;
    }
    snakeRect.left *= CELLWIDTH;
    snakeRect.top = snakeRect.top * CELLWIDTH;
    snakeRect.right = (snakeRect.right + 1) * CELLWIDTH;
    snakeRect.bottom = (snakeRect.bottom + 1) * CELLWIDTH;
}

bool CheckCollision(SNAKEV &snake)
{
    // If snake is outside walls
    if (snake.at(0).x < 0 || TOTALXCELLS - 1 < snake.at(0).x || snake.at(0).y < 0 || TOTALYCELLS - 1 < snake.at(0).y)
    {
        snake.erase(snake.begin());
        return true;
    }
    snake.pop_back();
    // If snake is overlapping with self
    for (int i = 1; i < snake.size(); i++)
    {
        if (snake.at(0).x == snake.at(i).x && snake.at(0).y == snake.at(i).y)
        {
            return true;
        }
    }
    return false;
}

void UpdateSnake(SNAKEV &snake, DIRECTION direction, LOCONGRID *foodLoc, RECT &snakeRect, GAMESTATE &gameState)
{
    LOCONGRID newSegment = snake.at(0);
    switch (direction)
    {
    case ID_MOVELEFT:
        newSegment.x -= 1;
        break;
    case ID_MOVEDOWN:
        newSegment.y += 1;
        break;
    case ID_MOVERIGHT:
        newSegment.x += 1;
        break;
    case ID_MOVEUP:
        newSegment.y -= 1;
        break;
    default:
        break;
    }
    snake.insert(snake.begin(), newSegment);
    if (snake.at(0).x == (*foodLoc).x && snake.at(0).y == (*foodLoc).y)
    {
        SetFoodLoc(foodLoc, snake);
        SetRect(&snakeRect, 0, 0, GAMEWIDTH, GAMEHEIGHT);
    }
    else
    {
        InvalidateSnakeRegion(snakeRect, snake);
        if (CheckCollision(snake))
        {
            // Game Over
            gameState = ID_GAMEOVER;
            SetRect(&snakeRect, 0, 0, GAMEWIDTH, GAMEHEIGHT);
        }
    }
}

LOCINPIXELS GridToPixel(LOCONGRID loc)
{
    LOCINPIXELS locPxl;
    locPxl.x = (CELLWIDTH / 2) + loc.x * CELLWIDTH;
    locPxl.y = (CELLWIDTH / 2) + loc.y * CELLWIDTH;
    return locPxl;
}

void SetFoodLoc(LOCONGRID *foodLoc, SNAKEV snake)
{
    bool validLocation = false;
    srand(time(NULL));
    while (!validLocation)
    {
        (*foodLoc).x = rand() % TOTALXCELLS;
        (*foodLoc).y = rand() % TOTALYCELLS;
        validLocation = true;
        for (LOCONGRID bodySegment : snake)
        {
            if (bodySegment.x == (*foodLoc).x && bodySegment.y == (*foodLoc).y)
            {
                validLocation = false;
                break;
            }
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

void PaintSnake(HDC hdc, SNAKEV snake)
{
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(0, 130, 0));
    for (int i = 0; i < snake.size() - 1; i++)
    {
        LOCINPIXELS currentLocPxl = GridToPixel(snake.at(i));
        LOCINPIXELS nextLocPxl = GridToPixel(snake.at(i + 1));
        DIRECTION direction = NextBodySegmentDirection(snake, i);
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

void PaintGame(HDC hdc, LOCONGRID foodLoc, SNAKEV snake)
{
    // PaintGrid(hdc);
    PaintSnake(hdc, snake);
    PaintFood(hdc, foodLoc);
}

void PaintCountdown(HDC hdc, int num)
{
    char pText[1];
    sprintf(pText, "%d", num);
    DisplayTextCenteredMiddle(hdc, pText);
}

void PaintGameOverScreen(HDC hdc, int score)
{
    char pGameOverText[100];
    sprintf(pGameOverText, "Game Over\n\nScore:\n%d", score);
    DisplayTextCenteredMiddle(hdc, pGameOverText);
}