/*
 * platform.c
 * --------
 * Bridge between game logic and Win32 rendering.
 * Owns the application lifecycle (init, message loop, shutdown) and routes input/timers
 * between game.c and snakeWin32.c.
 */


#include "platform.h"
#include "snakeWin32.h"
#include "game.h"
#include "log.h"

/* ===============================
   Lifecycle
================================== */
BOOL platform_Init(HINSTANCE hInstance, int nCmdShow)
{
    gameSetup();  // pure game state init (no Win32)
    // --- Create windows, brushes, fonts, handlers ---
    windowSetup(hInstance);

    if (!windowHandler.mainWindow) {
        return FALSE;
    }

    ShowWindow(windowHandler.mainWindow, nCmdShow);
    UpdateWindow(windowHandler.mainWindow);
    win32SetOnCloseCallback((onWindowCloseCallbackFn)platform_Shutdown);

    // --- Start Win32 message loop ---
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

void platform_Shutdown(void) {
    freeGameData();
}

void platform_ResetGame(void) {
    resetGame(windowHandler.gameFieldWindow);
}

void platform_InitializeCellAndNodeData(void) {
    initializeCellAndNodeData();
}


/* ===============================
   Rendering
================================== */
void platform_Repaint(void);

void platform_LoadScoreFont(HFONT scoreFont) {
    gameBoard.scoreFont = scoreFont;
}

BOOL platform_IsScoreFontNull(void) {
    return gameBoard.scoreFont == NULL;
}

void platform_LoadEnergyFont(HFONT energyFont) {
    gameBoard.energyFont = energyFont;
}

BOOL platform_IsEnergyFontNull(void) {
    return gameBoard.energyFont == NULL;
}

void platform_RepaintEnergyDisplay(void) {
    InvalidateRect(windowHandler.gameEnergyWindow, NULL, TRUE);
}


/* ===============================
   Logs
================================== */
void platform_ResetLogs(void) {
    resetLogs();
}

void platform_LogErrorMessage(const char* string) {
    int len = (int)strlen(string) + 2;  // +2 for newline and null chars
    wchar_t* errMsg = (wchar_t*)malloc(sizeof(wchar_t) * len);
    swprintf(errMsg, len, L"%hs\n", string);
    logError(errMsg);
    free(errMsg);
    exit(1);
}

void platform_LogDebugMessage(const char* string) {
    int len = (int)strlen(string) + 2;  // +2 for newline and null chars
    wchar_t* debugMsg = (wchar_t*)malloc(sizeof(wchar_t) * len);
    swprintf(debugMsg, len, L"%hs\n", string);
    logDebugMessage(debugMsg);
    free(debugMsg);
}


/* ===============================
   API - Platform -> Game
================================== */
void platform_TogglePause(void) {
    togglePause(windowHandler.mainWindow);
}

void platform_SetDirection(int direction) {
    gameBoard.snake.movement_direction = direction;
}

void platform_SetGameStatus_startGame() {
    gameBoard.gameStatus = START_GAME;
}

BOOL platform_IsStartGame() {
    return gameBoard.gameStatus == START_GAME;
}

void platform_SetGameStatus_pauseGame() {
    gameBoard.gameStatus = PAUSE_GAME;
}

BOOL platform_IsPauseGame() {
    return gameBoard.gameStatus == PAUSE_GAME;
}

void platform_SetGameStatus_gameOver() {
    gameBoard.gameStatus = GAME_OVER;
}

BOOL platform_IsGameOver() {
    return gameBoard.gameStatus == GAME_OVER;
}

BOOL platform_IsUpdateScore() {
    return gameBoard.update_score;
}

void platform_ToggleUpdateScore(void) {
    gameBoard.update_score = !gameBoard.update_score;
}

void platform_EnableBoost(void) {
    enableBoost(windowHandler.mainWindow);
}

void platform_DisableBoost(void) {
    disableBoost(windowHandler.mainWindow);
}

void platform_EnableBoostRecharge(void) {
    gameBoard.snake.boost_recharging = TRUE;
    setGameTimer(windowHandler.mainWindow, GAME_TIMER_BOOST_RECHARGE_ID);
}

void platform_DisableBoostRecharge(void) {
    gameBoard.snake.boost_recharging = FALSE;
    disableGameTimer(windowHandler.mainWindow, GAME_TIMER_BOOST_RECHARGE_ID);
}

BOOL platform_IsBoost(void) {
    return gameBoard.snake.boost;
}

BOOL platform_IsBoostDepleted(void) {
    return gameBoard.energy_level <= 0;
}

BOOL platform_IsBoostRecharging(void) {
    return gameBoard.snake.boost_recharging;
}

void platform_UpdateScoreText(void) {
    swprintf(gameBoard.score_text, 20, L"%s%d", gameBoard.score_label, gameBoard.score);
}

void platform_UpdateScoreDisplay(void) {
    InvalidateRect(windowHandler.gameDataDisplayWindow, NULL, TRUE);
    platform_ToggleUpdateScore();
}

int platform_GetGameBoardWidth(void) {
    return GAMEBOARDWIDTH;
}

int platform_GetGameBoardHeight(void) {
    return GAMEBOARDHEIGHT;
}

RECT platform_GetGameBoardRect(void) {
    GameBoardRect gameboardRect = getGameboardRect();
    RECT rect = {
        .left = gameboardRect.left,
        .top = gameboardRect.top,
        .right = gameboardRect.right,
        .bottom = gameboardRect.bottom
    };
    return rect;
}

int platform_GetTimerVal_Boost(void) {
    return GAME_TIMER_BOOST_VAL;
}

void platform_UpdateGameFieldWindow(void) {
    InvalidateRect(windowHandler.gameFieldWindow, NULL, TRUE);
}

void platform_UpdateEnergy(void) {
    updateEnergyLevel(windowHandler.mainWindow);
    InvalidateRect(windowHandler.gameEnergyWindow, NULL, TRUE);
}

int platform_GetGameEnergyLevel(void) {
    return gameBoard.energy_level;
}

HFONT platform_GetGameEnergyFont(void) {
    return gameBoard.energyFont;
}

void platform_UpdateGameboardPosition(void) {
    RECT mainWindowRect;
    GetClientRect(windowHandler.mainWindow, &mainWindowRect);
    updateGameboard(mainWindowRect);
}

void platform_UpdateGameboardWindow(void) {
    platform_UpdateGameboardPosition();
    GameBoardRect gameboardRect = getGameboardRect();
    MoveWindow(windowHandler.gameFieldWindow, gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height, TRUE);
}

HFONT platform_GetGameScoreFont(void) {
    return gameBoard.scoreFont;
}

wchar_t* platform_GetGameScoreText(void) {
    return gameBoard.score_text;
}

Coord platform_GetSnakeNodeIteratorCoord(void) {
    return (Coord){gameBoard.snake.node->x, gameBoard.snake.node->y};
}

void platform_ResetSnakeNodeIterator(void) {
    gameBoard.snakeNodeIterator = gameBoard.snake.node;
}

BOOL platform_IncrementSnakeNodeIterator(void) {
    if (gameBoard.snakeNodeIterator->nextNode == NULL) {
        gameBoard.snakeNodeIterator = gameBoard.snake.node;
        return FALSE;
    }
    gameBoard.snakeNodeIterator = gameBoard.snakeNodeIterator->nextNode;
    return TRUE;
}

Coord platform_GetFruitCoord(void) {
    return gameBoard.fruitLoc;
}

int platform_GetGameBoardCellWidth(void) {
    return getGameBoardCellWidth();
}

int platform_GetGameBoardCellHeight(void) {
    return getGameBoardCellHeight();
}





/* ===============================
   API - Game -> Platform
================================== */
void platform_GenerateNextFrame(HWND hwnd) {
    generateNextFrame(hwnd);
}

BOOL platform_IsSnakeNull(void) {
    return gameBoard.snake.node == NULL;
}

RECT platform_GetCellBoundingRect(Coord cellCoord) {
    RECT rect;
    rect.right = cellCoord.x * gameBoard.cell_width;
    rect.left = rect.right - gameBoard.cell_width;
    rect.bottom = cellCoord.y * gameBoard.cell_height;
    rect.top = rect.bottom - gameBoard.cell_height;
    return rect;
}
RECT platform_GetNodeBoundingRect(Coord nodeCoord) {
    RECT rect;
    rect.right = ((nodeCoord.x + 1) * gameBoard.cell_width);
    rect.left = rect.right - gameBoard.snake.node_diameter;
    rect.bottom = ((nodeCoord.y + 1) * gameBoard.cell_height);
    rect.top = rect.bottom - gameBoard.snake.node_diameter;
    return rect;
}
RECT platform_GetNodeInvalidationRect(Coord nodeCoord) {
    RECT rect;
    rect.right = ((nodeCoord.x + 1) * gameBoard.cell_width) + gameBoard.cell_width;
    rect.left = (rect.right - gameBoard.snake.node_diameter) - gameBoard.cell_width;
    rect.bottom = ((nodeCoord.y + 1) * gameBoard.cell_height) + gameBoard.cell_height;
    rect.top = (rect.bottom - gameBoard.snake.node_diameter) - gameBoard.cell_height;
    return rect;
}

void platform_UpdateFruitLocation(Coord prev_fruit_coord) {
    RECT prevFruitRect = platform_GetNodeInvalidationRect(prev_fruit_coord);
    RECT newFruitRect = platform_GetNodeInvalidationRect(gameBoard.fruitLoc);
    InvalidateRect(windowHandler.gameFieldWindow, &prevFruitRect, FALSE);
    InvalidateRect(windowHandler.gameFieldWindow, &newFruitRect, FALSE);
}

void platform_UpdateSnakeHeadLocation(Coord prev_head_coord, Coord new_head_coord) {
    RECT prevHeadRect = platform_GetNodeInvalidationRect(prev_head_coord);
    RECT newHeadRect = platform_GetNodeInvalidationRect(new_head_coord);
    InvalidateRect(windowHandler.gameFieldWindow, &prevHeadRect, FALSE);
    InvalidateRect(windowHandler.gameFieldWindow, &newHeadRect, FALSE);
}

void platform_UpdateSnakeTailLocation(Coord prev_tail_coord, Coord new_tail_coord) {
    RECT prevTailRect = platform_GetNodeInvalidationRect(prev_tail_coord);
    RECT newTailRect = platform_GetNodeInvalidationRect(new_tail_coord);
    InvalidateRect(windowHandler.gameFieldWindow, &prevTailRect, FALSE);
    InvalidateRect(windowHandler.gameFieldWindow, &newTailRect, FALSE);
}



