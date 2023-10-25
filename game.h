#ifndef UNICODE
    #define UNICODE
#endif

#include <windows.h>
#include <vector>
using namespace std;

// Game State Identifiers
#define ID_GAMEOVER     -4
#define ID_SPLASHSCREEN -3
#define ID_GAMESTARTING -2
#define ID_GAMEPAUSE     0
#define ID_GAMEPAUSING   1
#define ID_GAMEPLAY      2

// Game Dimensions
#define GAMEWIDTH  400
#define GAMEHEIGHT 400
#define CELLWIDTH     20
#define CELLHALFWIDTH 10
#define CONTENTRADIUS  8

// Timer
#define GAMETIMERINTERVAL 200
#define ID_GAMETIMER  360
#define ID_PAUSETIMER 380

// Display Text Function
void DisplayCenteredText(HDC hdc, char &message, int distFromTop);
void DisplayTextCenteredMiddle(HDC hdc, char &message);

// Grid Attributes
typedef struct { int x; int y; } LOCONGRID, LOCINPIXELS;
void SetFoodLoc(LOCONGRID* foodLoc, vector<LOCONGRID> snake);

// Directional Input
// The left bit is the x-direction and the right bit is the y-direction
#define ID_MOVELEFT  -0b10
#define ID_MOVEDOWN   0b01
#define ID_MOVERIGHT  0b10
#define ID_MOVEUP    -0b01
void KeyboardHandler(HWND hwnd, WPARAM key, int *direction, vector<LOCONGRID> snake, int &gameState);
void UpdateSnake(vector<LOCONGRID> &snake, int direction, LOCONGRID *foodLoc, RECT &snakeRect);

// Paint the Game
void PaintGame(HDC hdc, LOCONGRID foodLoc, vector<LOCONGRID> snake);
void PaintGameOverScreen(HDC hdc, int score);