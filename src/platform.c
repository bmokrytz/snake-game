
#include "platform.h"
#include "snakeWin32.h"
#include "game.h"
#include "log.h"

/* ===============================
   Lifecycle
================================== */
BOOL platform_Init(HINSTANCE hInstance, int nCmdShow)
{
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

void platform_Shutdown(void)
{
    freeGameData();
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


/* ===============================
   API - Platform -> Game
================================== */
void platform_TogglePause(void) {
    togglePause(windowHandler.mainWindow);
}

void platform_SetDirection(int direction) {
    snake.movement_direction = direction;
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
    snake.boost_recharging = TRUE;
    setGameTimer(windowHandler.mainWindow, GAME_TIMER_BOOST_RECHARGE_ID);
}

void platform_DisableBoostRecharge(void) {
    snake.boost_recharging = FALSE;
    disableGameTimer(windowHandler.mainWindow, GAME_TIMER_BOOST_RECHARGE_ID);
}

BOOL platform_IsBoost(void) {
    return snake.boost;
}

BOOL platform_IsBoostDepleted(void) {
    return gameBoard.energy_level <= 0;
}

BOOL platform_IsBoostRecharging(void) {
    return snake.boost_recharging;
}

void platform_UpdateScoreText(void) {
    swprintf(gameBoard.score_text, 20, L"%s%d", gameBoard.score_label, gameBoard.score);
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









/* ===============================
   API - Game -> Platform
================================== */
void platform_GenerateNextFrame(HWND hwnd) {
    generateNextFrame(hwnd);
}

BOOL platform_IsSnake(void) {
    return snake.node == NULL;
}

RECT platform_getCellBoundingRect(int x, int y) {
    RECT rect;
    rect.right = x * gameBoard.cell_width;
    rect.left = rect.right - gameBoard.cell_width;
    rect.bottom = y * gameBoard.cell_height;
    rect.top = rect.bottom - gameBoard.cell_height;
    return rect;
}
RECT platform_getNodeBoundingRect(int x, int y) {
    RECT rect;
    rect.right = ((x + 1) * gameBoard.cell_width);
    rect.left = rect.right - snake.node_diameter;
    rect.bottom = ((y + 1) * gameBoard.cell_height);
    rect.top = rect.bottom - snake.node_diameter;
    return rect;
}
RECT platform_getNodeInvalidationRect(int x, int y) {
    RECT rect;
    rect.right = ((x + 1) * gameBoard.cell_width) + gameBoard.cell_width;
    rect.left = (rect.right - snake.node_diameter) - gameBoard.cell_width;
    rect.bottom = ((y + 1) * gameBoard.cell_height) + gameBoard.cell_height;
    rect.top = (rect.bottom - snake.node_diameter) - gameBoard.cell_height;
    return rect;
}

void platform_UpdateFruitLocation(Coord prev_fruit_coord) {
    RECT prevFruitRect = platform_getNodeInvalidationRect(prev_fruit_coord.x, prev_fruit_coord.y);
    RECT newFruitRect = platform_getNodeInvalidationRect(gameBoard.fruitLoc.x, gameBoard.fruitLoc.y);
    InvalidateRect(windowHandler.gameFieldWindow, &prevFruitRect, FALSE);
    InvalidateRect(windowHandler.gameFieldWindow, &newFruitRect, FALSE);
}

void platform_UpdateSnakeHeadLocation(Coord prev_head_coord, Coord new_head_coord) {
    RECT prevHeadRect = platform_getNodeInvalidationRect(prev_head_coord.x, prev_head_coord.y);
    RECT newHeadRect = platform_getNodeInvalidationRect(new_head_coord.x, new_head_coord.y);
    InvalidateRect(windowHandler.gameFieldWindow, &prevHeadRect, FALSE);
    InvalidateRect(windowHandler.gameFieldWindow, &newHeadRect, FALSE);
}

void platform_UpdateSnakeTailLocation(Coord prev_tail_coord, Coord new_tail_coord) {
    RECT prevTailRect = platform_getNodeInvalidationRect(prev_tail_coord.x, prev_tail_coord.y);
    RECT newTailRect = platform_getNodeInvalidationRect(new_tail_coord.x, new_tail_coord.y);
    InvalidateRect(windowHandler.gameFieldWindow, &prevTailRect, FALSE);
    InvalidateRect(windowHandler.gameFieldWindow, &newTailRect, FALSE);
}



