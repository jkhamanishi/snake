#ifndef UNICODE
    #define UNICODE
#endif

#include <windows.h>
#include <vector>
using namespace std;

// Game State Identifiers
#define ID_GAMEOVER      -4
#define ID_NEXTGAMEREADY -3
#define ID_SPLASHSCREEN  -2
#define ID_GAMESTARTING  -1
#define ID_GAMEPAUSE      0
#define ID_GAMEPLAY       2
typedef int GAMESTATE;
void PlayPause(HWND hwnd, GAMESTATE &gameState);

// Game Dimensions
#define GAMEWIDTH  400
#define GAMEHEIGHT 400
#define CELLWIDTH     20
#define CONTENTRADIUS  8
#define TOTALXCELLS (GAMEWIDTH / CELLWIDTH)
#define TOTALYCELLS (GAMEHEIGHT / CELLWIDTH)

// Timer
#define ID_GAMETIMER      340
#define GAMETIMERINTERVAL 200
#define ID_COUNTDOWN      380
#define COUNTDOWNINTERVAL 800
#define ID_GAMEOVERTIMER  360
#define NEXTGAMEWAIT     2000

// Display Text Function
void DisplayCenteredText(HDC hdc, const char *message, int distFromTop);
void DisplayTextCenteredMiddle(HDC hdc, const char *message);

// Grid Attributes
typedef struct location { int x; int y; } LOCONGRID, LOCINPIXELS;
typedef vector<LOCONGRID> SNAKEV;
void SetFoodLoc(LOCONGRID* foodLoc, SNAKEV snake);

// Directional Input
// The left bit is the x-direction and the right bit is the y-direction
#define ID_MOVELEFT  -0b10
#define ID_MOVEDOWN   0b01
#define ID_MOVERIGHT  0b10
#define ID_MOVEUP    -0b01
typedef int DIRECTION;
typedef struct controls { bool wasd; bool arrowkeys; } ENABLECONTROLS;
void KeyboardHandler(WPARAM key, DIRECTION &direction, SNAKEV snake, ENABLECONTROLS enabledControls);
void UpdateSnake(SNAKEV &snake, DIRECTION direction, LOCONGRID *foodLoc, RECT &snakeRect, GAMESTATE &gameState);

// Paint the Game
void PaintGame(HDC hdc, LOCONGRID foodLoc, SNAKEV snake);
void PaintCountdown(HDC hdc, int num);
void PaintGameOverScreen(HDC hdc, int score);