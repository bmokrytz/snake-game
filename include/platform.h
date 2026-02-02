#ifndef PLATFORM_H
#define PLATFORM_H

#include <windows.h>

// Lifecycle
BOOL platform_Init(HINSTANCE hInstance, int nCmdShow);
void platform_Shutdown(void);

// Rendering
void platform_Repaint(void);

// Logs
void platform_ResetLogs(void);


// API - Platform -> Game
void platform_SetDirection(int direction);
void platform_SetGameStatus_startGame();
BOOL platform_IsStartGame();
void platform_SetGameStatus_pauseGame();
BOOL platform_IsPauseGame();
void platform_SetGameStatus_gameOver();
BOOL platform_IsGameOver();
void platform_EnableBoost(void);
void platform_DisableBoost(void);
void platform_EnableBoostRecharge(void);
void platform_DisableBoostRecharge(void);
BOOL platform_IsBoost(void);
BOOL platform_IsBoostDepleted(void);
BOOL platform_IsBoostRecharging(void);
void platform_UpdateScoreText(void);

// API - Game -> Platform
void platform_GenerateNextFrame(HWND hwnd);

#endif
