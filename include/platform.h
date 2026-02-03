#ifndef PLATFORM_H
#define PLATFORM_H

#include <windows.h>

// Lifecycle
BOOL platform_Init(HINSTANCE hInstance, int nCmdShow);
void platform_Shutdown(void);

// Rendering
void platform_Repaint(void);
void platform_LoadScoreFont(HFONT scoreFont);
BOOL platform_IsScoreFontNull(void);
void platform_LoadEnergyFont(HFONT energyFont);
BOOL platform_IsEnergyFontNull(void);

// Logs
void platform_ResetLogs(void);
void platform_LogErrorMessage(const char string);


// API - Platform -> Game
void platform_TogglePause(void);
void platform_SetDirection(int direction);
void platform_SetGameStatus_startGame(void);
BOOL platform_IsStartGame(void);
void platform_SetGameStatus_pauseGame(void);
BOOL platform_IsPauseGame(void);
void platform_SetGameStatus_gameOver(void);
BOOL platform_IsGameOver(void);
BOOL platform_IsUpdateScore(void);
void platform_ToggleUpdateScore(void);
void platform_EnableBoost(void);
void platform_DisableBoost(void);
void platform_EnableBoostRecharge(void);
void platform_DisableBoostRecharge(void);
BOOL platform_IsBoost(void);
BOOL platform_IsBoostDepleted(void);
BOOL platform_IsBoostRecharging(void);
void platform_UpdateScoreText(void);
int platform_GetGameBoardWidth(void);
int platform_GetGameBoardHeight(void);
RECT platform_GetGameBoardRect(void);
int platform_GetTimerVal_Boost(void);

// API - Game -> Platform
void platform_GenerateNextFrame(HWND hwnd);
BOOL platform_IsSnake(void);
RECT platform_getCellBoundingRect(int x, int y);
RECT platform_getNodeBoundingRect(int x, int y);
RECT platform_getNodeInvalidationRect(int x, int y);

void platform_UpdateFruitLocation(Coord prev_fruit_coord);

void platform_UpdateSnakeHeadLocation(Coord prev_head_coord, Coord new_head_coord);
void platform_UpdateSnakeTailLocation(Coord prev_tail_coord, Coord new_tail_coord);

#endif
