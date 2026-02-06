/*
 * game.h
 * ------
 * Public interface for the core Snake game logic and state.
 * Declares functions to initialize, update, and reset the game.
 *
 * No Win32/windowing or rendering code should be included here.
 */

#include "gameTimers.h"
#include "gameMacros.h"
#include "gameStructs.h"

#ifndef GAME_H
#define GAME_H

// ==================== Types ====================



// ==================== Globals ====================
// These must be *defined* in exactly one .c file (game.c), not in the header.
extern GameBoard gameBoard;

// ==================== Public API ====================

/*----------------------------------------------------------------------------*/
/*                                Game Setup                                  */
/*----------------------------------------------------------------------------*/
void gameSetup(void);                 // wrapper
void initializeGame(void);
void initializeGameGrid(void);
void initializeRand(void);
void initializeSnake(void);
void initializeFruit(void);
void initializeCellAndNodeData(void);

SnakeNode* createSnakeNode(SnakeNode config);

/*----------------------------------------------------------------------------*/
/*                                 Game Loop                                  */
/*----------------------------------------------------------------------------*/
void generateNextFrame(HWND hwnd);    // wrapper
void togglePause(HWND hwnd);

void enableBoost(HWND hwnd);
void disableBoost(HWND hwnd);

void updateEnergyLevel(HWND hwnd);

void generateFruit(HWND hwnd);
Coord generateCoordinate(void);

void eatFruit(HWND hwnd);
void extendSnake(HWND hwnd);
void moveSnake(HWND hwnd);

void changeSnakeDirection(int direction);
void incrementScore(void);
int collisionCheck(void);

// --- Utility ---
int getGameBoardCellWidth(void);
int getGameBoardCellHeight(void);

void setGameSpeed(int gameTimerID);
GameBoardRect getGameboardRect(void);

void updateGameboard(RECT mainWindowRect);

void setGameTimer(HWND hwnd, int gameTimerID);
void disableGameTimer(HWND hwnd, int gameTimerID);

/*----------------------------------------------------------------------------*/
/*                                Game Cleanup                                */
/*----------------------------------------------------------------------------*/
void freeGameData(void);              // wrapper
void freeSnake(void);

void resetGame(HWND hwnd);
void resetGameGrid(void);
void resetSnake(void);

/*----------------------------------------------------------------------------*/
/*                                  Helpers                                   */
/*----------------------------------------------------------------------------*/
int countSnakeTailNodes(void);

#endif // GAME_H
