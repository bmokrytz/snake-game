#include <stdio.h>
#include "error.h"
#include <windows.h>
#include "window.h"
#include "game.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    resetLogs();
    gameSetup();
    windowSetup(hInstance);
    ShowWindow(mainWindow, nCmdShow);
    //SetTimer(mainWindow, 1, 100, NULL);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

