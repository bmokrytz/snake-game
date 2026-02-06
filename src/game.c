/*
 * game.c
 * --------
 * Owns all core game state and logic for Snake.
 * Handles initialization, game loop updates, collision, scoring, and snake movement.
 * Does NOT handle rendering or platform/window logic.
 *
 * Used by: platform.c
 */

#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "platform.h"

// ==================== Globals ====================

GameBoard gameBoard;


// ******************** Function Implementations ********************
/*   --- Setup ---   */

void gameSetup() {
    initializeGame();
    initializeRand();
    initializeSnake();
    initializeFruit();
}

void initializeGame() {
    gameBoard.gameStatus = PAUSE_GAME;
    gameBoard.update_score = FALSE;
    gameBoard.score = 0;
    gameBoard.score_increment = 10;
    gameBoard.energy_level = 100;
    setGameSpeed(GAME_TIMER_NORMAL_SPEED_ID);
    swprintf(gameBoard.score_label, 10, L"Score: ");
    swprintf(gameBoard.score_text, 20, L"%s%d", gameBoard.score_label, gameBoard.score);
    initializeGameGrid();
}

void initializeGameGrid() {
    int rows = GAMEGRIDROWS, cols = GAMEGRIDCOLS;
    gameBoard.grid = (GameBoardGrid)malloc(sizeof(GridCell*) * rows);
    for (int i = 0; i < rows; i++) {
        gameBoard.grid[i] = (GridCell*)malloc(sizeof(GridCell) * cols);
        for (int j = 0; j < cols; j++) {
            gameBoard.grid[i][j].x = i; gameBoard.grid[i][j].y = j;
            gameBoard.grid[i][j].containsHead = 0;
            gameBoard.grid[i][j].containsSnake = 0;
            gameBoard.grid[i][j].containsFruit = 0;
            if (i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) gameBoard.grid[i][j].containsWall = 1;
            else gameBoard.grid[i][j].containsWall = 0;
        }
    }
    gameBoard.grid_rows = rows;
    gameBoard.grid_cols = cols;
}

void initializeRand() {
    srand((unsigned int)time(NULL));
}

void initializeSnake() {
    gameBoard.snake.node = createSnakeNode(
        (SnakeNode)
        {
            .x = SNAKEHEADSTARTX,
            .y = SNAKEHEADSTARTY,
            .prev_x = SNAKEHEADSTARTX,
            .prev_y = SNAKEHEADSTARTY,
            .prevNode = NULL,
            .nextNode = NULL
        }
    );
    gameBoard.snakeNodeIterator = gameBoard.snake.node;

    if (platform_IsSnakeNull()) {
        platform_LogErrorMessage("Error in function setupSnakeHead() of game.h.\n\tplatform_IsSnakeNull() == TRUE. Malloc failed in createSnakeNode().\n");
    }

    gameBoard.grid[SNAKEHEADSTARTX][SNAKEHEADSTARTY].containsHead = 1;
    gameBoard.snake.movement_direction = DIRECTION_UP;
    gameBoard.snake.boost = FALSE;
    gameBoard.snake.boost_recharging = FALSE;
}

void initializeFruit() {
    gameBoard.fruitLoc = generateCoordinate();
    gameBoard.grid[gameBoard.fruitLoc.x][gameBoard.fruitLoc.y].containsFruit = 1;
}


void initializeCellAndNodeData() {
    if ((GAMEBOARDWIDTH % GAMEGRIDCOLS || GAMEBOARDHEIGHT % GAMEGRIDROWS) != 0) {
        platform_LogErrorMessage("Error in function setupGridCellDimensions() of game.h.\n\t(gameBoard.rect.width \% gameBoard.grid_cols) != 0\n");
    }
    gameBoard.cell_width = (gameBoard.rect.width / gameBoard.grid_cols);
    gameBoard.cell_height = (gameBoard.rect.height / gameBoard.grid_rows);
    gameBoard.snake.node_diameter = gameBoard.cell_width * 2;
}


SnakeNode* createSnakeNode(SnakeNode config) {
    SnakeNode* node = (SnakeNode*)malloc(sizeof(SnakeNode));
    if (node == NULL) {
        platform_LogErrorMessage("Error in function createSnakeNode() of game.h.\n\tnode == NULL. Malloc failed.\n");
    }

    node->x = config.x;
    node->y = config.y;
    node->prev_x = config.prev_x;
    node->prev_y = config.prev_y;
    node->nextNode = config.nextNode;
    node->prevNode = config.prevNode;

    return node;
}



/*   --- Game Loop ---   */


void generateNextFrame(HWND hwnd) {
    moveSnake(hwnd);
    int collisionVal = collisionCheck();

    switch (collisionVal) {
        case COLLISION:
            gameBoard.gameStatus = GAME_OVER;
            break;

        case EATS_FRUIT:
            eatFruit(hwnd);
            break;
    }
}


void togglePause(HWND hwnd) {
    if (gameBoard.gameStatus == PAUSE_GAME) {
        gameBoard.gameStatus = START_GAME;
        setGameTimer(hwnd, gameBoard.game_timer_id);
        return;
    } else {
        gameBoard.gameStatus = PAUSE_GAME;
        KillTimer(hwnd, gameBoard.game_timer_id);
    }
}

void enableBoost(HWND hwnd) {
    if (gameBoard.gameStatus == START_GAME && gameBoard.snake.boost == FALSE) {
        gameBoard.snake.boost = TRUE;
        setGameTimer(hwnd, GAME_TIMER_BOOST_ID);
    }
}

void disableBoost(HWND hwnd) {
    if (gameBoard.gameStatus == START_GAME && gameBoard.snake.boost == TRUE) {
        gameBoard.snake.boost = FALSE;
        disableGameTimer(hwnd, GAME_TIMER_BOOST_ID);
    }
}

void updateEnergyLevel(HWND hwnd) {
    if (gameBoard.snake.boost == TRUE) {
        gameBoard.energy_level -= ENERGY_DECAY_VALUE;
        if (gameBoard.energy_level < 0) {
            gameBoard.energy_level = 0;
        }
    }
    else {
        gameBoard.energy_level += ENERGY_REPLENISH_VALUE;
        if (gameBoard.energy_level > 100) {
            gameBoard.energy_level = 100;
            platform_DisableBoostRecharge();
        }
    }
}


void generateFruit(HWND hwnd) {
    gameBoard.fruitLoc = generateCoordinate();
    gameBoard.grid[gameBoard.fruitLoc.x][gameBoard.fruitLoc.y].containsFruit = 1;
}


Coord generateCoordinate() {
    Coord coordinate;
    BOOL coordinate_invalid = TRUE;
    while (coordinate_invalid) {
        coordinate_invalid = FALSE;
        coordinate.x = (rand() % (GAMEGRIDCOLS - (2 * FRUIT_MARGIN))) + FRUIT_MARGIN;
        coordinate.y = (rand() % (GAMEGRIDROWS - (2 * FRUIT_MARGIN))) + FRUIT_MARGIN;
        for (int i = 0 - COLLISION_RANGE; i <= 0 + COLLISION_RANGE; i++) {
            for (int j = 0 - COLLISION_RANGE; j <= 0 + COLLISION_RANGE; j++) {
                if (coordinate.x + i == gameBoard.snake.node->x && coordinate.y + j == gameBoard.snake.node->y) {
                    coordinate_invalid = TRUE;
                    break;
                }
            }
            if (coordinate_invalid) {
                break;  
            }
        }
    }
    return coordinate;
}


void eatFruit(HWND hwnd) {
    int prev_fruit_x = gameBoard.fruitLoc.x;
    int prev_fruit_y = gameBoard.fruitLoc.y;
    gameBoard.grid[gameBoard.fruitLoc.x][gameBoard.fruitLoc.y].containsFruit = 0;
    incrementScore();
    extendSnake(hwnd);
    generateFruit(hwnd);
    platform_UpdateFruitLocation((Coord){prev_fruit_x, prev_fruit_y});
}


void extendSnake(HWND hwnd) {
    SnakeNode* ptr = gameBoard.snake.node;
    while (ptr->nextNode != NULL) {
        ptr = ptr->nextNode;
    }
    ptr->nextNode = createSnakeNode(
        (SnakeNode)
        {
            .x = ptr->prev_x,
            .y = ptr->prev_y,
            .prev_x = ptr->prev_x,
            .prev_y = ptr->prev_y,
            .prevNode = ptr,
            .nextNode = NULL
        }
    );
    platform_UpdateSnakeTailLocation((Coord){ptr->x, ptr->y}, (Coord){ptr->nextNode->x, ptr->nextNode->y});
}


void moveSnake(HWND hwnd) {
    if (platform_IsSnakeNull()) {
        platform_LogErrorMessage("Error in function moveSnake() of game.h.\n\tplatform_IsSnakeNull() == TRUE\n");
    }
    Coord prev_head_coord = {0, 0};
    Coord new_head_coord = {0, 0};
    Coord prev_tail_coord = {0, 0};
    Coord new_tail_coord = {0, 0};

    SnakeNode* ptr = gameBoard.snake.node;
    prev_head_coord.x = ptr->x; prev_head_coord.y = ptr->y;
    ptr->prev_x = ptr->x;
    ptr->prev_y = ptr->y;
    gameBoard.grid[ptr->x][ptr->y].containsHead = 0;

    if (gameBoard.snake.movement_direction == DIRECTION_UP) {
        ptr->y -= 1;
    } else if (gameBoard.snake.movement_direction == DIRECTION_DOWN) {
        ptr->y += 1;
    } else if (gameBoard.snake.movement_direction == DIRECTION_LEFT) {
        ptr->x -= 1;
    } else if (gameBoard.snake.movement_direction == DIRECTION_RIGHT) {
        ptr->x += 1;
    }
    new_head_coord.x = ptr->x; new_head_coord.y = ptr->y;
    gameBoard.grid[ptr->x][ptr->y].containsHead = 1;
    
    ptr = ptr->nextNode;
    while (ptr != NULL) {
        ptr->prev_x = ptr->x;
        ptr->prev_y = ptr->y;
        gameBoard.grid[ptr->x][ptr->y].containsSnake = 0;

        ptr->x = ptr->prevNode->prev_x;
        ptr->y = ptr->prevNode->prev_y;

        gameBoard.grid[ptr->x][ptr->y].containsSnake = 1;

       if (ptr->nextNode == NULL) {
        prev_tail_coord.x = ptr->prev_x; prev_tail_coord.y = ptr->prev_y;
        new_tail_coord.x = ptr->x; new_tail_coord.y = ptr->y; 
       }
        ptr = ptr->nextNode;
    }
    platform_UpdateSnakeHeadLocation(prev_head_coord, new_head_coord);
    platform_UpdateSnakeTailLocation(prev_tail_coord, new_tail_coord);
}


void changeSnakeDirection(int direction) {
    gameBoard.snake.movement_direction = direction;
}


void incrementScore() {
    gameBoard.score += gameBoard.score_increment;
    swprintf(gameBoard.score_text, 20, L"%s%d", gameBoard.score_label, gameBoard.score);
    gameBoard.update_score = TRUE;
}


int collisionCheck() {
    if (platform_IsSnakeNull()) {
        platform_LogErrorMessage("Error in function collisionCheck() of game.h.\n\tplatform_IsSnakeNull() == TRUE\n");
    }
    if (gameBoard.grid[gameBoard.snake.node->x][gameBoard.snake.node->y].containsSnake) {
        return COLLISION;
    }
    for (int i = 0 - COLLISION_RANGE; i <= 0 + COLLISION_RANGE; i++) {
        for (int j = 0 - COLLISION_RANGE; j <= 0 + COLLISION_RANGE; j++) {
            if (gameBoard.grid[gameBoard.snake.node->x + i][gameBoard.snake.node->y + j].containsWall) {
                if (i != 0 && j != 0) {
                    return COLLISION;
                }
            }
            else if (gameBoard.grid[gameBoard.snake.node->x + i][gameBoard.snake.node->y + j].containsFruit) {
                return EATS_FRUIT;
            }
        }
    }
    return NO_COLLISION;
}

void resetGame(HWND hwnd) {
    gameBoard.score = 0;
    gameBoard.energy_level = 100;
    resetGameGrid();
    resetSnake();
    gameBoard.grid[gameBoard.fruitLoc.x][gameBoard.fruitLoc.y].containsFruit = 0;
    generateFruit(hwnd);
    RECT wndRect; GetClientRect(hwnd, &wndRect);
    InvalidateRect(hwnd, &wndRect, TRUE);
}

void resetGameGrid() {
    int rows = GAMEGRIDROWS, cols = GAMEGRIDCOLS;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gameBoard.grid[i][j].containsHead = 0;
            gameBoard.grid[i][j].containsSnake = 0;
            gameBoard.grid[i][j].containsFruit = 0;
            if (i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) gameBoard.grid[i][j].containsWall = 1;
            else gameBoard.grid[i][j].containsWall = 0;
        }
    }
}

/*   --- Utility ---   */


int getGameBoardCellWidth() {
    return gameBoard.cell_width;
}


int getGameBoardCellHeight() {
    return gameBoard.cell_height;
}

void setGameSpeed(int gameTimerID) {
    switch (gameTimerID) {
        case (GAME_TIMER_NORMAL_SPEED_ID):
            gameBoard.game_timer_id = GAME_TIMER_NORMAL_SPEED_ID;
            break;
        case (GAME_TIMER_SLOW_SPEED_ID):
            gameBoard.game_timer_id = GAME_TIMER_SLOW_SPEED_ID;
            break;
        case (GAME_TIMER_FAST_SPEED_ID):
            gameBoard.game_timer_id = GAME_TIMER_FAST_SPEED_ID;
            break;
    }
}


GameBoardRect getGameboardRect() {
    GameBoardRect copy = {
        .bottom = gameBoard.rect.bottom,
        .height = gameBoard.rect.height,
        .left = gameBoard.rect.left,
        .right = gameBoard.rect.right,
        .top = gameBoard.rect.top,
        .width = gameBoard.rect.width
    };
    return copy;
}


void updateGameboard(RECT mainWindowRect) {
    int mainWindowWidth = mainWindowRect.right - mainWindowRect.left;
    int mainWindowHeight = mainWindowRect.bottom - mainWindowRect.top;
    gameBoard.rect.height = GAMEBOARDHEIGHT; gameBoard.rect.width = GAMEBOARDWIDTH;
    gameBoard.rect.left   = (mainWindowWidth  - gameBoard.rect.width) / 2;
    gameBoard.rect.top    = (mainWindowHeight - gameBoard.rect.height) / 2;
    gameBoard.rect.right  = gameBoard.rect.left + gameBoard.rect.width;
    gameBoard.rect.bottom = gameBoard.rect.top + gameBoard.rect.height;
    if ((GAMEBOARDWIDTH % GAMEGRIDCOLS || GAMEBOARDHEIGHT % GAMEGRIDROWS) != 0) {
        platform_LogErrorMessage("Error in function setupGridCellDimensions() of game.h.\n\t(gameBoard.rect.width \% gameBoard.grid_cols) != 0\n");
    }
    gameBoard.cell_width = (gameBoard.rect.width / gameBoard.grid_cols);
    gameBoard.cell_height = (gameBoard.rect.height / gameBoard.grid_rows);
}


void setGameTimer(HWND hwnd, int gameTimerID) {
    switch (gameTimerID) {
        case (GAME_TIMER_NORMAL_SPEED_ID):
            SetTimer(hwnd, GAME_TIMER_NORMAL_SPEED_ID, GAME_TIMER_NORMAL_SPEED_VAL, NULL);
            break;
        case (GAME_TIMER_SLOW_SPEED_ID):
            SetTimer(hwnd, GAME_TIMER_SLOW_SPEED_ID, GAME_TIMER_SLOW_SPEED_VAL, NULL);
            break;
        case (GAME_TIMER_FAST_SPEED_ID):
            SetTimer(hwnd, GAME_TIMER_FAST_SPEED_ID, GAME_TIMER_FAST_SPEED_VAL, NULL);
            break;
        case (GAME_TIMER_BOOST_ID):
            SetTimer(hwnd, GAME_TIMER_BOOST_ID, GAME_TIMER_BOOST_VAL, NULL);
            break;
        case (GAME_TIMER_BOOST_RECHARGE_ID):
            SetTimer(hwnd, GAME_TIMER_BOOST_RECHARGE_ID, GAME_TIMER_BOOST_RECHARGE_VAL, NULL);
            break;
    }
}

void disableGameTimer(HWND hwnd, int gameTimerID) {
    switch (gameTimerID) {
        case (GAME_TIMER_NORMAL_SPEED_ID):
            KillTimer(hwnd, GAME_TIMER_NORMAL_SPEED_ID);
            break;
        case (GAME_TIMER_SLOW_SPEED_ID):
            KillTimer(hwnd, GAME_TIMER_SLOW_SPEED_ID);
            break;
        case (GAME_TIMER_FAST_SPEED_ID):
            KillTimer(hwnd, GAME_TIMER_FAST_SPEED_ID);
            break;
        case (GAME_TIMER_BOOST_ID):
            KillTimer(hwnd, GAME_TIMER_BOOST_ID);
            break;
        case (GAME_TIMER_BOOST_RECHARGE_ID):
            KillTimer(hwnd, GAME_TIMER_BOOST_RECHARGE_ID);
            break;
    }
}

/*   -------------   */

/*   --- Clean Up ---   */


void freeGameData() {
    for (int i = 0; i < GAMEGRIDROWS; i++) {
        free(gameBoard.grid[i]);
    }
    free(gameBoard.grid);
    freeSnake();
}


void freeSnake() {
    if (platform_IsSnakeNull()) {
        platform_LogErrorMessage("Error in function freeSnake() of game.h.\n\tplatform_IsSnakeNull() == TRUE\n");
    }
    SnakeNode* node = gameBoard.snake.node;
    SnakeNode* freeNode = gameBoard.snake.node;
    while(node != NULL) {
        node = node->nextNode;
        free(freeNode);
        freeNode = node;
    }
    gameBoard.snake.node = NULL;
}

int countSnakeTailNodes() {
    SnakeNode* node = gameBoard.snake.node->nextNode;
    int counter = 0;
    while (node != NULL) {
        counter++;
        node = node->nextNode;
    }
    return counter;
}

void resetSnake() {
    if (platform_IsSnakeNull()) {
        platform_LogErrorMessage("Error in function resetSnake() of game.h.\n\tplatform_IsSnakeNull() == TRUE\n");
    }

    SnakeNode* node = gameBoard.snake.node;
    SnakeNode* freeNode = node;
    int tailNodes = countSnakeTailNodes();
    int counter = 1;

    while(node != NULL) {
        node = node->nextNode;
        counter++;
        free(freeNode);
        freeNode = node;
    }

    initializeSnake();
}

/* ************************************************************ */
