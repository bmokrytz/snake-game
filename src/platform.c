
#include "platform.h"
#include "snakeWin32.h"
#include "game.h"
#include "log.h"

/* ===============================
   Lifecycle
================================== */
BOOL platformInit(HINSTANCE hInstance, int nCmdShow)
{
    // --- Create windows, brushes, fonts, handlers ---
    windowSetup(hInstance);

    if (!windowHandler.mainWindow) {
        return FALSE;
    }

    ShowWindow(windowHandler.mainWindow, nCmdShow);
    UpdateWindow(windowHandler.mainWindow);
    win32SetOnCloseCallback((onWindowCloseCallbackFn)platformShutdown);

    // --- Start Win32 message loop ---
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

void platformShutdown(void)
{
    freeGameData();
}

// Rendering
void platformRepaint(void);

// Logs
void platform_ResetLogs(void) {
    resetLogs();
}

// API - Platform -> Game
void platform_SetDirection(int direction) {
    snake.movement_direction = direction;
}

void platform_SetGameStatus_startGame() {
    gameBoard.gameStatus = START_GAME;
}

BOOL platform_IsStartGame() {
    if (gameBoard.gameStatus == START_GAME) {
        return TRUE;
    }
    return FALSE;
}

void platform_SetGameStatus_pauseGame() {
    gameBoard.gameStatus = PAUSE_GAME;
}

BOOL platform_IsPauseGame() {
    if (gameBoard.gameStatus == PAUSE_GAME) {
        return TRUE;
    }
    return FALSE;
}

void platform_SetGameStatus_gameOver() {
    gameBoard.gameStatus = GAME_OVER;
}

BOOL platform_IsGameOver() {
    if (gameBoard.gameStatus == GAME_OVER) {
        return TRUE;
    }
    return FALSE;
}

void platform_SetBoost(void);

void platform_DisableBoost(HWND hwnd) {
    disableBoost(hwnd);
}

void platform_EnableBoostRecharge(void) {
    snake.boost_recharging = TRUE;`
    setGameTimer(windowHandler.mainWindow, GAME_TIMER_BOOST_RECHARGE_ID);
}

void platform_DisableBoostRecharge(void) {
    snake.boost_recharging = FALSE;
    disableGameTimer(windowHandler.mainWindow, GAME_TIMER_BOOST_RECHARGE_ID);
}

BOOL platform_IsBoost(void) {
    if (snake.boost) {
        return TRUE;
    }
    return FALSE;
}

BOOL platform_IsBoostDepleted(void) {
    if (snake.boost_depleted) {
        return TRUE;
    }
    return FALSE;
}

BOOL platform_IsBoostRecharging(void) {
    if (snake.boost_recharging) {
        return TRUE;
    }
    return FALSE;
}

void platform_UpdateScoreText(void) {
    swprintf(gameBoard.score_text, 20, L"%s%d", gameBoard.score_label, gameBoard.score);
}


// API - Game -> Platform
void platform_GenerateNextFrame(HWND hwnd) {
    generateNextFrame(hwnd);
}