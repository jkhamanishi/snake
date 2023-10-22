#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "style.h"

DWORD SetWindowStyle()
{
    // Disables resizing and maximizing
    return WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
}

HICON LoadCustomIcon()
{
    return (HICON)LoadImage(NULL, L"snake.ico", IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
}

HMENU LoadGameMenu()
{
    // Main Menu
    HMENU hMenu = CreateMenu();
    // Game Menu
    HMENU hGameMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hGameMenu, L"&Game");
    AppendMenu(hGameMenu, MF_STRING, IDM_PLAYPAUSE, L"&Play/Pause\tEsc");
    AppendMenu(hGameMenu, MF_STRING, IDM_EXIT, L"&Exit\tAlt+F4");
    // Return Main Menu
    return hMenu;
}
