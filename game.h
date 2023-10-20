#include <windows.h>

// Game State Identifiers
#define ID_GAMEOVER     -3
#define ID_SPLASHSCREEN -2
#define ID_GAMESTARTING -1
#define ID_GAMEPAUSE     0
#define ID_GAMEPLAY      1

// Game Dimensions
RECT GameRect();
#define GAMEWIDTH  500
#define GAMEHEIGHT 500
#define CELLWIDTH    25
#define CONTENTWIDTH 20

// Display Text Function
void DisplayTextCenteredMiddle(HWND hwnd, LPCTSTR message);
void DisplayCenteredText(HWND hwnd, LPCTSTR message, int distFromTop);

// Directional Input
#define ID_MOVELEFT  1
#define ID_MOVEDOWN  2
#define ID_MOVERIGHT 3
#define ID_MOVEUP    4
void KeyboardHandler(WPARAM key, int& direction);