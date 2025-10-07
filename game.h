#ifndef GAME_H
#define GAME_H


#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "error.h"
#include "window.h"



// ******************** Macros ********************

#ifndef GAME_MACROS
#define GAME_MACROS
#define GAMEBOARDWIDTH 600
#define GAMEBOARDHEIGHT 600
#define GAMEGRIDROWS 60
#define GAMEGRIDCOLS 60
#define SNAKEHEADSTARTX (GAMEGRIDCOLS / 2)
#define SNAKEHEADSTARTY (GAMEGRIDROWS / 2)
#define SCORE_INCREMENT 10
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3
#define EATS_FRUIT 4
#define COLLISION 5
#define NO_COLLISION 6
#define PAUSE_GAME 7
#define START_GAME 8
#define GAME_OVER 9
#endif

/* ************************************************************ */

// ******************** Structs ********************

typedef struct GameBoardRect {
    int top;
    int bottom;
    int left;
    int right;
    int width;
    int height;
} GameBoardRect;

typedef struct GridCell {
    int x;
    int y;
    int containsHead;
    int containsSnake;
    int containsWall;
    int containsFruit;
} GridCell;
typedef GridCell** GameBoardGrid;

typedef struct GameBoard {
    HWND window;
    GameBoardRect rect;
    GameBoardGrid grid;
    int grid_rows;
    int grid_cols;
    int cell_width;
    int cell_height;
} GameBoard;

typedef struct SnakeNode {
    int x;
    int y;
    int prev_x;
    int prev_y;
    struct SnakeNode* prevNode;
    struct SnakeNode* nextNode;
} SnakeNode;

typedef struct SnakeHead {
    SnakeNode* node;
    int movement_direction;
} SnakeHead;

typedef struct Coord {
    int x;
    int y;
} Coord;

// ******************** Typedefs ********************







/* ************************************************************ */

// ******************** Global Variables ********************

GameBoard gameBoard;
SnakeHead snake;
int score;
int gameStatus;

/* ************************************************************ */

// ******************** Function Prototypes ********************

/*   --- Setup ---   */
void gameSetup(); // - Wrapper
void initializeGame();
void initializeGameGrid();
void initializeRand();
void initializeSnake();
SnakeNode* createSnakeNode(SnakeNode config);
/*   -------------   */

/*   --- Game Loop ---   */
void generateNextFrame(); // - Wrapper
void togglePause();
void generateFruit();
Coord generateCoordinate();
void eatFruit();
void extendSnake();
void moveSnake();
void changeSnakeDirection(int direction);
void incrementScore();
int collisionCheck();
/*   -------------   */

/*   --- Utility ---   */
int getGameBoardCellWidth();
int getGameBoardCellHeight();
GameBoardRect getGameboardRect();
void updateGameboard(RECT mainWindowRect);
RECT getCellBoundingRect(int x, int y);
/*   -------------   */

/*   --- Clean Up ---   */
void freeGameData(); // - Wrapper
void freeSnake();
/*   -------------   */

/* ************************************************************ */


// ******************** Function Implementations ********************

/*   --- Setup ---   */
void gameSetup() {
    initializeGame();
    initializeRand();
    initializeSnake();
}

void initializeGame() {
    score = 0; gameStatus = PAUSE_GAME;
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
    snake.node = createSnakeNode(
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
    if (snake.node == NULL) {
        logError(L"Error in function setupSnakeHead() of game.h.\n\tsnake.node == NULL. Malloc failed in createSnakeNode().\n");
    }
    gameBoard.grid[SNAKEHEADSTARTX][SNAKEHEADSTARTY].containsHead = 1;
    snake.movement_direction = DIRECTION_UP;
}

SnakeNode* createSnakeNode(SnakeNode config) {
    SnakeNode* node = (struct SnakeNode*)malloc(sizeof(SnakeNode));
    if (node == NULL) {
        logError(L"Error in function createSnakeNode() of game.h.\n\tnode == NULL. Malloc failed.\n");
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
void generateNextFrame() {
    wchar_t errMsg[256];
    swprintf(errMsg, 256, L"running generateNextFrame()...\n");
    logDebugMessage(errMsg);
    moveSnake();
    int collisionVal = collisionCheck();
    switch (collisionVal) {
        case COLLISION:
        {
            gameStatus = GAME_OVER;
            break;
        }
        case EATS_FRUIT:
        {
            eatFruit();
            break;
        }
    }
}

void togglePause() {
    if (gameStatus == PAUSE_GAME) {
        gameStatus = START_GAME;
        return;
    }
    else {
        gameStatus = PAUSE_GAME;
    }
}

void generateFruit() {
    Coord fruitCoordinate = generateCoordinate();
    gameBoard.grid[fruitCoordinate.x][fruitCoordinate.y].containsFruit = 1;
}

Coord generateCoordinate() {
    Coord coordinate = {.x = snake.node->x, .y = snake.node->y};
    while (coordinate.x == snake.node->x) {
        coordinate.x = 1 + rand() % (GAMEGRIDCOLS - 1);
    }
    while (coordinate.y == snake.node->y) {
        coordinate.y = 1 + rand() % (GAMEGRIDROWS - 1);
    }
    return coordinate;
}

void eatFruit() {
    gameBoard.grid[snake.node->x][snake.node->y].containsFruit = 0;
    incrementScore();
    extendSnake();
    generateFruit();
}

void extendSnake() {
    SnakeNode* ptr = snake.node;
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
}

void moveSnake() {
    if (snake.node == NULL) {
        logError(L"Error in function moveSnake() of game.h.\n\tsnake.node == NULL\n");
    }
    SnakeNode* ptr = snake.node;
    ptr->prev_x = ptr->x;
    ptr->prev_y = ptr->y;
    gameBoard.grid[ptr->x][ptr->y].containsHead = 0;
    if (snake.movement_direction == DIRECTION_UP) {
        ptr->y -= 1;
    }
    else if (snake.movement_direction == DIRECTION_DOWN) {
        ptr->y += 1;
    }
    else if (snake.movement_direction == DIRECTION_LEFT) {
        ptr->x -= 1;
    }
    else if (snake.movement_direction == DIRECTION_RIGHT) {
        ptr->x += 1;
    }
    gameBoard.grid[ptr->x][ptr->y].containsHead = 1;
    ptr = ptr->nextNode;
    while (ptr != NULL) {
        ptr->prev_x = ptr->x;
        ptr->prev_y = ptr->y;
        gameBoard.grid[ptr->x][ptr->y].containsSnake = 0;
        ptr->x = ptr->prevNode->prev_x;
        ptr->y = ptr->prevNode->prev_y;
        gameBoard.grid[ptr->x][ptr->y].containsSnake = 1;
        ptr = ptr->nextNode;
    }
}

void changeSnakeDirection(int direction) {
    snake.movement_direction = direction;
}

void incrementScore() {
    score += SCORE_INCREMENT;
}

int collisionCheck() {
    if (snake.node == NULL) {
        logError(L"Error in function collisionCheck() of game.h.\n\tsnake.node == NULL\n");
    }
    if (gameBoard.grid[snake.node->x][snake.node->y].containsWall || gameBoard.grid[snake.node->x][snake.node->y].containsSnake) {
        return COLLISION;
    }
    else if (gameBoard.grid[snake.node->x][snake.node->y].containsFruit) {
        return EATS_FRUIT;
    }
    return NO_COLLISION;
}

/*   --- Utility ---   */
int getGameBoardCellWidth() {
    return gameBoard.cell_width;
}

int getGameBoardCellHeight() {
    return gameBoard.cell_height;
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
        logError(L"Error in function setupGridCellDimensions() of game.h.\n\t(gameBoard.rect.width \% gameBoard.grid_cols) != 0\n");
    }
    gameBoard.cell_width = (gameBoard.rect.width / gameBoard.grid_cols);
    gameBoard.cell_height = (gameBoard.rect.height / gameBoard.grid_rows);
}

RECT getCellBoundingRect(int x, int y) {
    RECT rect;
    rect.right = x * gameBoard.cell_width;
    rect.left = rect.right - gameBoard.cell_width;
    rect.bottom = y * gameBoard.cell_height;
    rect.top = rect.bottom - gameBoard.cell_height;
    return rect;
}

RECT getCellBoundingRect2(int x, int y) {
    RECT rect;
    rect.right = gameBoard.rect.left + (x * gameBoard.cell_width);
    rect.left = gameBoard.rect.left + (rect.right - gameBoard.cell_width);
    rect.bottom = gameBoard.rect.top + (y * gameBoard.cell_height);
    rect.top = gameBoard.rect.top + (rect.bottom - gameBoard.cell_height);
    return rect;
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
    if (snake.node == NULL) {
        logError(L"Error in function freeSnake() of game.h.\n\tsnake.node == NULL\n");
    }
    SnakeNode* node = snake.node;
    SnakeNode* freeNode = snake.node;
    while(node != NULL) {
        node = node->nextNode;
        free(freeNode);
        freeNode = node;
    }
    snake.node = NULL;
}


/* ************************************************************ */


#endif

