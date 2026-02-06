/*
 * platform.h
 * ----------
 * Public interface for the platform bridge layer.
 * Declares functions used to initialize the app, route input/timers, and expose
 * game state needed by the Win32 renderer.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include <windows.h>
#include "gameTimers.h"
#include "gameMacros.h"
#include "gameStructs.h"

// Lifecycle
BOOL platform_Init(HINSTANCE hInstance, int nCmdShow);
void platform_Shutdown(void);
void platform_ResetGame(void);
void platform_InitializeCellAndNodeData(void);

// Rendering
void platform_Repaint(void);
void platform_LoadScoreFont(HFONT scoreFont);
void platform_LoadEnergyFont(HFONT energyFont);
void platform_RepaintEnergyDisplay(void);

// Logs
void platform_ResetLogs(void);
void platform_LogErrorMessage(const char* string);
void platform_LogDebugMessage(const char* string);

// Game Controls
void platform_GenerateNextFrame(HWND hwnd);
void platform_TogglePause(void);
void platform_SetDirection(int direction);
void platform_SetGameStatus_startGame(void);
void platform_SetGameStatus_pauseGame(void);
void platform_SetGameStatus_gameOver(void);
void platform_ToggleUpdateScore(void);
void platform_EnableBoost(void);
void platform_DisableBoost(void);
void platform_EnableBoostRecharge(void);
void platform_DisableBoostRecharge(void);

// Predicate Functions
BOOL platform_IsSnakeNull(void);
BOOL platform_IsBoost(void);
BOOL platform_IsBoostDepleted(void);
BOOL platform_IsBoostRecharging(void);
BOOL platform_IsGameOver(void);
BOOL platform_IsUpdateScore(void);
BOOL platform_IsPauseGame(void);
BOOL platform_IsStartGame(void);
BOOL platform_IsScoreFontNull(void);
BOOL platform_IsEnergyFontNull(void);

// Score API
void platform_UpdateScoreText(void);
void platform_UpdateScoreDisplay(void);

// Game Data Getters
int platform_GetGameBoardWidth(void);
int platform_GetGameBoardHeight(void);
int platform_GetTimerVal_Boost(void);
int platform_GetGameEnergyLevel(void);
int platform_GetGameBoardCellWidth(void);
int platform_GetGameBoardCellHeight(void);
Coord platform_GetFruitCoord(void);
Coord platform_GetSnakeNodeIteratorCoord(void);
RECT platform_GetGameBoardRect(void);
RECT platform_GetCellBoundingRect(Coord cellCoord);
RECT platform_GetNodeBoundingRect(Coord nodeCoord);
RECT platform_GetNodeInvalidationRect(Coord nodeCoord);
HFONT platform_GetGameEnergyFont(void);
HFONT platform_GetGameScoreFont(void);
wchar_t* platform_GetGameScoreText(void);

// Update Functions
void platform_UpdateGameFieldWindow(void);
void platform_UpdateEnergy(void);
void platform_UpdateGameboardPosition(void);
void platform_UpdateGameboardWindow(void);
void platform_UpdateFruitLocation(Coord prev_fruit_coord);
void platform_UpdateSnakeHeadLocation(Coord prev_head_coord, Coord new_head_coord);
void platform_UpdateSnakeTailLocation(Coord prev_tail_coord, Coord new_tail_coord);

// Snake Node Iterator
void platform_ResetSnakeNodeIterator(void);
BOOL platform_IncrementSnakeNodeIterator(void);


#endif
