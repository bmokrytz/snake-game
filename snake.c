/**
 * @file main.c
 * @brief Entry point for the Snake game application.
 *
 * This module initializes the game systems, creates the main application
 * window, and starts the Win32 message loop. It ties together all major
 * components, including logging, game setup, and window management.
 *
 * Responsibilities:
 * - Initialize and reset log files.
 * - Set up game data and window resources.
 * - Display the main window and start the frame timer.
 * - Process incoming Windows messages until the program exits.
 *
 * @see gameSetup()
 * @see windowSetup()
 * @see wWinMain()
 */

#include <stdio.h>
#include "log.h"
#include <windows.h>
#include "window.h"
#include "game.h"

// Timer macros
#define GAME_TIMER_NORMAL_SPEED_ID 1
#define GAME_TIMER_NORMAL_SPEED_VAL 45
#define GAME_TIMER_SLOW_SPEED_ID 2
#define GAME_TIMER_SLOW_SPEED_VAL 45
#define GAME_TIMER_FAST_SPEED_ID 3
#define GAME_TIMER_FAST_SPEED_VAL 45

/**
 * @brief The Win32 application entry point for the Snake game.
 *
 * Initializes logging, sets up the game and window systems, and then enters
 * the main Windows message loop. A timer is created to drive periodic game
 * updates through the window procedure.
 *
 * @param hInstance      Handle to the current application instance.
 * @param hPrevInstance  Reserved; always NULL in modern Windows applications.
 * @param pCmdLine       Pointer to the command line string.
 * @param nCmdShow       Controls how the window is displayed (e.g. SW_SHOW).
 *
 * @return The exit code passed to PostQuitMessage() (typically 0).
 *
 * @note This function uses the wide-character (Unicode) version of WinMain.
 *
 * @see resetLogs()
 * @see gameSetup()
 * @see windowSetup()
 * @see ShowWindow()
 * @see SetTimer()
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    resetLogs();
    gameSetup();
    windowSetup(hInstance);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

