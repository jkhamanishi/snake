#include <windows.h>
#include <vector>
using namespace std;

// Game State Identifiers
#define ID_GAMEOVER     -3
#define ID_SPLASHSCREEN -2
#define ID_GAMESTARTING -1
#define ID_GAMEPAUSE     0
#define ID_GAMEPLAY      1
void PlayPause(HWND hwnd, int &gameState);

// Game Dimensions
RECT GameRect();
#define GAMEWIDTH  500
#define GAMEHEIGHT 500
#define CELLWIDTH     20
#define CELLHALFWIDTH 10
#define CONTENTRADIUS  8

// Timer
#define ID_TIMER 360
#define TIMERINTERVAL 200

// Display Text Function
void DisplayTextCenteredMiddle(HWND hwnd, LPCTSTR message);
void DisplayCenteredText(HWND hwnd, LPCTSTR message, int distFromTop);

// Grid Attributes
struct LOCONGRID { int x; int y; };
struct LOCINPIXELS { int x; int y; };
void SetFoodLoc(LOCONGRID* foodLoc, vector<LOCONGRID> snake);

// Directional Input
// The left bit is the x-direction and the right bit is the y-direction
#define ID_MOVELEFT  -0b10
#define ID_MOVEDOWN   0b01
#define ID_MOVERIGHT  0b10
#define ID_MOVEUP    -0b01
void KeyboardHandler(HWND hwnd, WPARAM key, int *direction, vector<LOCONGRID> snake, int &gameState);
void UpdateSnake(vector<LOCONGRID> &snake, int direction, RECT &snakeRect);

// Paint the Game
void GamePainter(HDC hdc, LOCONGRID foodLoc, vector<LOCONGRID> snake);