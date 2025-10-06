#include "window.h"
#include "game.h"
#include "error.h"
#include <stdio.h>

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Clear log files
    wipeDebugLog();
    wipeErrorLog();

    // Main window setup
    const wchar_t CLASS_NAME[]  = L"SNAKE";
    RegisterWindowClass(hInstance, CLASS_NAME, MainWindowProc);
    mainWindow = CreateMainWindow(CLASS_NAME, hInstance);
    if (mainWindow == NULL) return 0;
    
    // Game board setup
    setupGameBoard(mainWindow);

    // Game board window setup
    const wchar_t GAME_CLASS_NAME[]  = L"SNAKE GAME BOARD";
    RegisterWindowClass(hInstance, GAME_CLASS_NAME, GameWindowProc);
    gameWindow = CreateGameBoardWindow(mainWindow, GAME_CLASS_NAME, hInstance);

    // Toggle main window visibility
    ShowWindow(mainWindow, nCmdShow);

    // Set a timer for the game loop
    SetTimer(mainWindow, 1, 100, NULL);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

