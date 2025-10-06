#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "error.h"
#include <unistd.h>


// ******************** Macros ********************

/*
#define DEBUG

#ifdef DEBUG
    logDebugMessage(L"Running drawFrame()...\n\n");
#endif
*/

// Set color values for the game board
#define BACKGROUND_COLOR_VAL RGB(33, 176, 164)
#define FIELD_COLOR_VAL RGB(255, 255, 255)
#define SNAKE_LINK_COLOR_VAL RGB(38, 191, 51)
#define FRUIT_COLOR_VAL RGB(235, 232, 52)
#define WALL_COLOR_VAL RGB(0, 0, 0)
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

/* ************************************************************ */



// ******************** Structs ********************

/**
 * @brief Stores game board dimensions, width, and height.
 *
 * Member of GameBoard type.
 *
 * @param top       (int) - The position of the top of the board within the main window.
 * @param bottom    (int) - The position of the bottom of the board within the main window.
 * @param left      (int) - The position of the left side of the board within the main window.
 * @param right     (int) - The position of the right side of the board within the main window.
 * @param width     (int) - The pixel width of the board.
 * @param height    (int) - The pixel height of the board.
 * 
 */
typedef struct GameBoardRect {
    int top;
    int bottom;
    int left;
    int right;
    int width;
    int height;
} GameBoardRect;

/**
 * @brief An individual cell in the game board grid
 *
 * @param x                 (int) - The x coordinate of the cell.
 * @param y                 (int) - The y coordinate of the cell.
 * @param containsSnake     (int) - A flag for whether the snake is in this cell.
 * @param containsWall      (int) - A flag for whether this cell is a wall.
 * @param containsFruit     (int) - A flag for whether this cell is a fruit.
 * 
 */
typedef struct GridCell {
    int x;
    int y;
    int containsSnake;
    int containsWall;
    int containsFruit;
} GridCell;

typedef GridCell** GameBoardGrid; ///< Represents the full 2D board (2D array of GridCell pointers)

/**
 * @brief Stores the game board properties.
 *
 * @param rect          (GameBoardRect) - Stores game board dimensions, width, and height.
 * @param grid          (GameBoardGrid) - Stores the 2D grid of positions on the game board.
 * @param grid_rows     (int) - The number of rows in the game board grid.
 * @param grid_cols     (int) - The number of columns in the game board grid.
 * @param cell_width    (int) - The pixel width of the game grid cells.
 * @param cell_height   (int) - The pixel width of the game grid cells.
 * 
 */
typedef struct GameBoard {
    HWND window;
    GameBoardRect rect;
    GameBoardGrid grid;
    int grid_rows;
    int grid_cols;
    int cell_width;
    int cell_height;
} GameBoard;

/**
 * @brief An individual link of the snake's body.
 *
 * @param x             (int) - The x coordinate of the link.
 * @param y             (int) - The y coordinate of the link.
 * @param prev_x        (int) - The previous x coordinate of the link.
 * @param prev_y        (int) - The previous y coordinate of the link.
 * @param nextNode      (SnakeNode*) - Pointer to the node ahead of this one
 * @param prevNode      (SnakeNode*) - Pointer to the node behind this one.
 * 
 */
typedef struct SnakeNode {
    int x;
    int y;
    int prev_x;
    int prev_y;
    struct SnakeNode* nextNode;
    struct SnakeNode* prevNode;
} SnakeNode;

/**
 * @brief The head of the snake. Also the first link in the snake's body.
 *
 * @param node                  (SnakeNode*) - The node data for the snake head.
 * @param movement_direction    (int) - A flag for the movement direction of the snake.
 * 
 */
typedef struct SnakeHead {
    SnakeNode* node;
    int movement_direction;
} SnakeHead;

/**
 * @brief A fruit. Food for the snake.
 *
 * @param x     (int) - The x coordinate of the fruit.
 * @param y     (int) - The y coordinate of the fruit.
 * 
 */
typedef struct Fruit {
    int x;
    int y;
} Fruit;

//typedef void (*thread_func_t)(void*);

/*
typedef struct Thread_wrapper_arg {
    thread_func_t func;
    void* arg;
} Thread_wrapper_arg;
*/

/* ************************************************************ */






// ******************** Global Variables ********************

GameBoard gameBoard;
SnakeHead snake;
int score;
int gameStatus;
HBRUSH snakeBrush;
HBRUSH backgroundBrush;
HBRUSH fieldBrush;
HBRUSH wallBrush;
HBRUSH fruitBrush; 

/* ************************************************************ */





// ******************** Function Prototypes ********************

void setupGridCellDimensions();
void updateGameboardRect(HWND mainWindow);
GameBoardRect getGameBoardRect();
void setupGameBoardGrid();
void setupGameBoard(HWND mainWindow);
void setupSnakeHead();
SnakeNode* createSnakeNode(SnakeNode config);
void createNextNode(SnakeNode *prevNode);
void freeGameBoard();
void freeGameBoardGrid();
void freeSnake();
void updateNodePosition(SnakeNode *node);
void updateSnake();
void deleteBrushes();
void updateSnakeHeadPosition();
void togglePause();

void updateState(HWND mainWindow, HWND gameWindow);
void drawFrame(HWND mainWindow, HWND gameWindow);
void endGame();
int collisionCheck();




/* ************************************************************ */














// ******************** Function Definitions ********************

void togglePause() {
    if (gameStatus == PAUSE_GAME) {
        gameStatus = START_GAME;
    }
    else {
        gameStatus = PAUSE_GAME;
    }
}

void updateState(HWND mainWindow, HWND gameWindow) {
    updateSnake();
    drawFrame(mainWindow, gameWindow);
    int collisionValue = collisionCheck();
    if (collisionValue == COLLISION) {
        endGame();
    }
    else if (collisionValue == EATS_FRUIT) {
        score += SCORE_INCREMENT;
    }
    else if (collisionValue != NO_COLLISION) {
        logError(L"Error in function updateState(HWND mainWindow, HWND gameWindow) of game.h.\n\tUnknown collisionValue.\n");
    }
}

void updateSnake() {
    if (snake.node == NULL) return;
    updateSnakeHeadPosition();
    SnakeNode* node = snake.node->nextNode;
    while (node != NULL) {
        updateNodePosition(node);
        node = node->nextNode;
    }
}

void changeSnakeDirection(int direction) {
    snake.movement_direction = direction;
}

void endGame() {
    freeGameBoard();
    return;
}

void drawFrame(HWND mainWindow, HWND gameWindow) {
    InvalidateRect(mainWindow, NULL, TRUE);
    //UpdateWindow(mainWindow);
    InvalidateRect(gameWindow, NULL, TRUE);
    //UpdateWindow(gameWindow);
}


int collisionCheck() {
    int head_x = snake.node->x;
    int head_y = snake.node->y;
    if (gameBoard.grid[head_x][head_y].containsWall || gameBoard.grid[head_x][head_y].containsSnake) {
        return COLLISION;
    }
    else if (gameBoard.grid[head_x][head_y].containsFruit) {
        return EATS_FRUIT;
    }
    return NO_COLLISION;
}

RECT getCellBoundingRect(int x, int y) {
    RECT rect;
    int radius = gameBoard.cell_width / 2;
    rect.right = x * gameBoard.cell_width;
    rect.left = rect.right - gameBoard.cell_width;
    rect.bottom = y * gameBoard.cell_height;
    rect.top = rect.bottom - gameBoard.cell_height;
    return rect;
}

/**
 * @brief Wrapper function for setting up the game board.
 * 
 * This wrapper calls:
 * 
 * 1) void setupGameBoardRect(HWND mainWindow);
 * 
 * 2) void setupGameBoardGrid(int rows, int cols);
 * 
 * 3) void setupGridCellDimensions();
 *
 */
void setupGameBoard(HWND mainWindow) {
    score = 0; gameStatus = PAUSE_GAME;
    snakeBrush = CreateSolidBrush(SNAKE_LINK_COLOR_VAL);
    backgroundBrush = CreateSolidBrush(BACKGROUND_COLOR_VAL);
    fieldBrush = CreateSolidBrush(FIELD_COLOR_VAL);
    fruitBrush = CreateSolidBrush(FRUIT_COLOR_VAL);
    wallBrush = CreateSolidBrush(WALL_COLOR_VAL);
    setupSnakeHead();
    updateGameboardRect(mainWindow);
    setupGameBoardGrid();
    setupGridCellDimensions();
}

/**
 * @brief Setup function for the GameBoardRect which stores dimensons, 
 * width, and height of the game board.
 *
 * @param hwnd      (HWND) - Handle to the parent window.
 * @param width     (int) - The width of the game board.
 * @param height    (int) - The height of the game board.
 * 
 */
void updateGameboardRect(HWND mainWindow) {
    RECT mainWindowRect; GetClientRect(mainWindow, &mainWindowRect);
    int mainWindowWidth = mainWindowRect.right - mainWindowRect.left;
    int mainWindowHeight = mainWindowRect.bottom - mainWindowRect.top;
    gameBoard.rect.height = GAMEBOARDHEIGHT; gameBoard.rect.width = GAMEBOARDWIDTH;
    gameBoard.rect.left   = (mainWindowWidth  - gameBoard.rect.width) / 2;
    gameBoard.rect.top    = (mainWindowHeight - gameBoard.rect.height) / 2;
    gameBoard.rect.right  = gameBoard.rect.left + gameBoard.rect.width;
    gameBoard.rect.bottom = gameBoard.rect.top + gameBoard.rect.height;
}

/**
 * @brief Setup function for the GameBoardRect which stores dimensons, 
 * width, and height of the game board.
 *
 * @param hwnd      (HWND) - Handle to the parent window.
 * @param width     (int) - The width of the game board.
 * @param height    (int) - The height of the game board.
 * 
 */
void setupGameBoardGrid() {
    int rows = GAMEGRIDROWS, cols = GAMEGRIDCOLS;
    gameBoard.grid = (GameBoardGrid)malloc(sizeof(GridCell*) * rows);
    for (int i = 0; i < rows; i++) {
        gameBoard.grid[i] = (GridCell*)malloc(sizeof(GridCell) * cols);
        for (int j = 0; j < cols; j++) {
            gameBoard.grid[i][j].x = i; gameBoard.grid[i][j].y = j;
            gameBoard.grid[i][j].containsSnake = 0;
            gameBoard.grid[i][j].containsFruit = 0;
            if (i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) gameBoard.grid[i][j].containsWall = 1;
            else gameBoard.grid[i][j].containsWall = 0;
        }
    }
    gameBoard.grid_rows = rows;
    gameBoard.grid_cols = cols;
}

void freeGameBoard() {
    freeGameBoardGrid();
    deleteBrushes();
    freeSnake();
}

void deleteBrushes() {
    DeleteObject(snakeBrush);
    DeleteObject(backgroundBrush);
    DeleteObject(fieldBrush);
    DeleteObject(fruitBrush);
    DeleteObject(wallBrush);
}

void freeGameBoardGrid() {
    int rows = GAMEGRIDROWS, cols = GAMEGRIDCOLS;
    for (int i = 0; i < rows; i++) {
        free(gameBoard.grid[i]);
    }
    free(gameBoard.grid);
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

void setupSnakeHead() {
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

void createNextNode(SnakeNode *prevNode) {
    SnakeNode *node = createSnakeNode(
        (SnakeNode)
        {
            .x = prevNode->prev_x,
            .y = prevNode->prev_y,
            .prev_x = prevNode->prev_x,
            .prev_y = prevNode->prev_y,
            .prevNode = prevNode,
            .nextNode = NULL
        }
    );
    prevNode->nextNode = node;
}

void updateNodePosition(SnakeNode *node) {
    if (node == NULL) {
        logError(L"Error in function updateNodePosition() of game.h.\n\tnode == NULL.\n");
    }
    gameBoard.grid[node->x][node->y].containsSnake = 0;
    node->prev_x = node->x;
    node->prev_y = node->y;
    node->x = node->prevNode->prev_x;
    node->y = node->prevNode->prev_y;
    gameBoard.grid[node->x][node->y].containsSnake = 1;
}

void updateSnakeHeadPosition() {
    if (snake.movement_direction == DIRECTION_UP) {
        snake.node->y -= 1;
    }
    else if (snake.movement_direction == DIRECTION_LEFT) {
        snake.node->x -= 1;
    }
    else if (snake.movement_direction == DIRECTION_DOWN) {
        snake.node->y += 1;
    }
    else if (snake.movement_direction == DIRECTION_RIGHT) {
        snake.node->x += 1;
    }
    else {
        logError(L"Error in function updateSnakeHeadPosition() of game.h.\n\tUnknown value for snake.movement_direction.\n");
    }
}

GameBoardRect getGameBoardRect() {
    return gameBoard.rect;
}

int getGameBoardCellWidth() {
    return gameBoard.cell_width;
}

int getGameBoardCellHeight() {
    return gameBoard.cell_height;
}

void setupGridCellDimensions() {
    if ((GAMEBOARDWIDTH % GAMEGRIDCOLS || GAMEBOARDHEIGHT % GAMEGRIDROWS) != 0) {
        logError(L"Error in function setupGridCellDimensions() of game.h.\n\t(gameBoard.rect.width \% gameBoard.grid_cols) != 0\n");
    }
    gameBoard.cell_width = (gameBoard.rect.width / gameBoard.grid_cols);
    gameBoard.cell_height = (gameBoard.rect.height / gameBoard.grid_rows);
}

/* ************************************************************ */


#endif

