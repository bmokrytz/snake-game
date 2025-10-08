#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "log.h"
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

/**
 * @brief Defines the rectangular bounds and dimensions of the game board in pixels.
 *
 * Used to describe the visible or logical area where the game grid is drawn.
 */
typedef struct GameBoardRect {
    int top;      /**< Top boundary (Y-coordinate). */
    int bottom;   /**< Bottom boundary (Y-coordinate). */
    int left;     /**< Left boundary (X-coordinate). */
    int right;    /**< Right boundary (X-coordinate). */
    int width;    /**< Total width of the game board in pixels. */
    int height;   /**< Total height of the game board in pixels. */
} GameBoardRect;

/**
 * @brief Represents a single cell within the game grid.
 *
 * Each cell stores its grid coordinates and flags that describe
 * what occupies it (snake body, head, wall, or fruit).
 */
typedef struct GridCell {
    int x;              /**< Row position of the cell in the grid. */
    int y;              /**< Column position of the cell in the grid. */
    int containsHead;   /**< 1 if the snake's head is in this cell, otherwise 0. */
    int containsSnake;  /**< 1 if a snake body segment occupies this cell. */
    int containsWall;   /**< 1 if this cell represents a wall or border. */
    int containsFruit;  /**< 1 if a fruit is placed in this cell. */
} GridCell;

/**
 * @brief A 2D array type representing the full game grid.
 */
typedef GridCell** GameBoardGrid;

/**
 * @brief Holds all data related to the game board, including the grid and its dimensions.
 *
 * Contains references to the grid structure, its size in rows and columns,
 * and metadata needed for rendering (window handle, pixel cell size, etc.).
 */
typedef struct GameBoard {
    HWND window;            /**< Handle to the game window (Win32). */
    GameBoardRect rect;     /**< Pixel boundaries of the game area. */
    GameBoardGrid grid;     /**< 2D array representing all grid cells. */
    int grid_rows;          /**< Number of grid rows. */
    int grid_cols;          /**< Number of grid columns. */
    int cell_width;         /**< Width of each cell in pixels. */
    int cell_height;        /**< Height of each cell in pixels. */
} GameBoard;

/**
 * @brief Represents a single node (segment) of the snake's body.
 *
 * Each node stores its current and previous positions, as well as
 * pointers to adjacent nodes in the doubly linked list.
 */
typedef struct SnakeNode {
    int x;                     /**< Current X-coordinate (column index). */
    int y;                     /**< Current Y-coordinate (row index). */
    int prev_x;                /**< Previous X-coordinate before movement. */
    int prev_y;                /**< Previous Y-coordinate before movement. */
    struct SnakeNode* prevNode;/**< Pointer to the previous node in the list. */
    struct SnakeNode* nextNode;/**< Pointer to the next node in the list. */
} SnakeNode;

/**
 * @brief Represents the snake's head and direction of movement.
 *
 * Contains a pointer to the head node of the snake's linked body
 * and an integer indicating its current movement direction.
 */
typedef struct SnakeHead {
    SnakeNode* node;       /**< Pointer to the head node of the snake. */
    int movement_direction;/**< Current movement direction (e.g., up, down, left, right). */
    int node_diameter;
} SnakeHead;

/**
 * @brief Represents a simple 2D coordinate pair.
 *
 * Useful for passing positions without referencing full grid cells or nodes.
 */
typedef struct Coord {
    int x; /**< X-coordinate value. */
    int y; /**< Y-coordinate value. */
} Coord;

/* ************************************************************ */

// ******************** Global Variables ********************

/**
 * @brief Represents the current game board state.
 *
 * Holds the grid, its dimensions, and related information
 * that defines the playable area and its contents.
 *
 * @see GameBoard
 */
GameBoard gameBoard;

/**
 * @brief Represents the snake's head and movement state.
 *
 * Stores the head position, direction, and links to the rest
 * of the snake's body segments.
 *
 * @see SnakeHead
 */
SnakeHead snake;

/**
 * @brief Current score for the active game session.
 *
 * Tracks the player's score, which increases when the snake
 * eats fruit. Reset to zero when a new game starts.
 */
int score;

/**
 * @brief Tracks the current status of the game.
 *
 * Indicates whether the game is running, paused, or over.
 * Uses the predefined constants:
 * - START_GAME
 * - PAUSE_GAME
 * - GAME_OVER
 */
int gameStatus;

Coord gameFruit;

/* ************************************************************ */

// ******************** Function Prototypes ********************

/*   --- Setup ---   */
void gameSetup(); // - Wrapper
void initializeGame();
void initializeGameGrid();
void initializeRand();
void initializeSnake();
void initializeCellAndNodeData();
SnakeNode* createSnakeNode(SnakeNode config);
/*   -------------   */

/*   --- Game Loop ---   */
int generateNextFrame(); // - Wrapper
void togglePause();
void generateFruit();
Coord generateCoordinate();
int eatFruit();
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
RECT getNodeBoundingRect(int x, int y);
/*   -------------   */

/*   --- Clean Up ---   */
void freeGameData(); // - Wrapper
void freeSnake();
/*   -------------   */

/* ************************************************************ */


// ******************** Function Implementations ********************
/*   --- Setup ---   */

/**
 * @brief Performs full game setup by initializing all core systems.
 *
 * This function serves as a wrapper that initializes all major components
 * required before starting gameplay. It ensures the random number generator,
 * game grid, and snake are properly set up.
 *
 * Specifically, it calls:
 * - initializeGame() – sets up the game board and global state.
 * - initializeRand() – seeds the random number generator.
 * - initializeSnake() – creates and positions the initial snake.
 *
 * @note Must be called once before entering the main game loop.
 *
 * @see initializeGame()
 * @see initializeRand()
 * @see initializeSnake()
 */
void gameSetup() {
    initializeGame();
    initializeRand();
    initializeSnake();
    generateFruit();
}

/**
 * @brief Initializes core game data and state.
 *
 * This function sets up the fundamental game components by:
 * - Resetting the global score to zero.
 * - Setting the initial game status to paused.
 * - Allocating and initializing the game grid via initializeGameGrid().
 *
 * It prepares the game for its first run or for restarting after a previous session.
 *
 * @note Must be called before starting or resuming gameplay.
 *
 * @see initializeGameGrid()
 */
void initializeGame() {
    score = 0; gameStatus = PAUSE_GAME;
    initializeGameGrid();
}

/**
 * @brief Allocates and initializes the game grid structure.
 *
 * This function dynamically allocates memory for the 2D game grid and
 * initializes each cell with its coordinates and default state values.
 * 
 * - Borders (first and last rows/columns) are marked as walls.
 * - All other cells are initialized as empty (no snake, no fruit).
 * 
 * The grid dimensions are taken from the GAMEGRIDROWS and GAMEGRIDCOLS macros,
 * and the resulting grid is stored in the global `gameBoard` structure.
 *
 * @note This function must be called before any operations that access
 *       or modify the game grid.
 */
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

/**
 * @brief Initializes the random number generator with the current time.
 *
 * This function seeds the standard library's pseudo-random number generator
 * using the current system time, ensuring that calls to rand() produce a
 * different sequence of values each time the program runs.
 */
void initializeRand() {
    srand((unsigned int)time(NULL));
}

/**
 * @brief Initializes the snake at its starting position on the game board.
 *
 * This function creates the initial snake head node at the center of the grid,
 * sets its movement direction to up, and marks the corresponding grid cell as
 * containing the snake head. It prepares the snake for gameplay immediately
 * after the board has been initialized.
 *
 * If memory allocation fails during snake node creation, an error message is
 * logged via logError().
 *
 * @note Must be called after initializeGameGrid() to ensure the grid exists.
 *
 * @see createSnakeNode()
 * @see logError()
 */
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

/**
 * @brief Initializes derived cell and node size data based on the game board dimensions.
 *
 * Calculates the pixel width and height of each grid cell by dividing the game board's
 * total width and height by the number of grid columns and rows, respectively.
 * Also sets the snake's node diameter relative to the cell width.
 *
 * Logs an error if the board dimensions are not evenly divisible by the grid size,
 * which would indicate a misconfigured game board layout.
 *
 * @note This function should be called after the game board rectangle and grid
 *       dimensions have been initialized (typically during setup or resizing).
 *
 * @see updateGameboard()
 * @see logError()
 */
void initializeCellAndNodeData() {
    if ((GAMEBOARDWIDTH % GAMEGRIDCOLS || GAMEBOARDHEIGHT % GAMEGRIDROWS) != 0) {
        logError(L"Error in function setupGridCellDimensions() of game.h.\n\t(gameBoard.rect.width \% gameBoard.grid_cols) != 0\n");
    }
    gameBoard.cell_width = (gameBoard.rect.width / gameBoard.grid_cols);
    gameBoard.cell_height = (gameBoard.rect.height / gameBoard.grid_rows);
    snake.node_diameter = gameBoard.cell_width * 2;
}

/**
 * @brief Allocates and initializes a new SnakeNode based on a configuration template.
 *
 * Dynamically allocates memory for a new SnakeNode and copies position and linkage
 * data from the provided configuration struct. This function is used both to
 * create the snake's head and to append new body segments during gameplay.
 *
 * If memory allocation fails, an error message is logged via logError().
 *
 * @param config A SnakeNode structure providing initial values for the new node.
 * @return Pointer to the newly allocated SnakeNode, or NULL if allocation fails.
 *
 * @see SnakeNode
 * @see logError()
 */
SnakeNode* createSnakeNode(SnakeNode config) {
    SnakeNode* node = (SnakeNode*)malloc(sizeof(SnakeNode));
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

/**
 * @brief Advances the game state by one frame and handles movement, collisions, and fruit events.
 *
 * This function updates the game state for the next frame by moving the snake,
 * checking for collisions, and processing fruit consumption. If the snake collides
 * with itself or a wall, the game status is set to GAME_OVER. If the snake eats a fruit,
 * the fruit logic is handled, and the function checks whether the eaten fruit was near
 * the edge of the board.
 *
 * @return int
 * Returns 1 if the fruit was eaten near the game board boundary (indicating walls may
 * need to be repainted), otherwise returns 0.
 *
 * @see moveSnake()
 * @see collisionCheck()
 * @see eatFruit()
 */
int generateNextFrame(void) {
    moveSnake();
    int collisionVal = collisionCheck();

    switch (collisionVal) {
        case COLLISION:
            gameStatus = GAME_OVER;
            break;

        case EATS_FRUIT:
            if (eatFruit() == 1) {
                return 1;
            }
            break;
    }
    return 0;
}

/**
 * @brief Toggles the game's pause state.
 *
 * Switches the global gameStatus variable between paused and running modes.
 * If the game is currently paused, it resumes gameplay; otherwise, it pauses.
 *
 * @see gameStatus
 */
void togglePause(void) {
    if (gameStatus == PAUSE_GAME) {
        gameStatus = START_GAME;
        return;
    } else {
        gameStatus = PAUSE_GAME;
    }
}

/**
 * @brief Generates a new fruit at a random coordinate on the grid.
 *
 * Selects a random coordinate using generateCoordinate() and marks that
 * position as containing a fruit. Ensures that the new fruit does not spawn
 * at the snake's current position or at a wall position.
 *
 * @see generateCoordinate()
 */
void generateFruit(void) {
    gameFruit = generateCoordinate();
    gameBoard.grid[gameFruit.x][gameFruit.y].containsFruit = 1;
}

/**
 * @brief Generates a random grid coordinate that does not overlap with the snake's head or a wall.
 *
 * Produces a random (x, y) coordinate within the playable grid area,
 * ensuring that the position does not coincide with the snake's current
 * head location. Used for fruit placement.
 *
 * @return A Coord structure containing the generated grid position.
 *
 * @see Coord
 */
Coord generateCoordinate(void) {
    Coord coordinate = { .x = snake.node->x, .y = snake.node->y };

    while (coordinate.x == snake.node->x) {
        coordinate.x = 1 + rand() % (GAMEGRIDCOLS - 1);
    }
    while (coordinate.y == snake.node->y) {
        coordinate.y = 1 + rand() % (GAMEGRIDROWS - 1);
    }

    return coordinate;
}

/**
 * @brief Handles logic for when the snake eats a fruit and signals wall repainting if needed.
 *
 * Removes the fruit from its current cell, increases the player's score,
 * extends the snake's length by one segment, and generates a new fruit at a
 * random location. After processing, it checks whether the eaten fruit was
 * positioned near the edges of the game board.
 *
 * If the fruit was close to the boundary, the function returns a signal so the
 * caller can trigger a wall repaint (to ensure proper redraw after fruit overlap).
 *
 * @return int
 * Returns 1 if the eaten fruit was within two cells of any game board edge
 * (indicating walls should be repainted), otherwise returns 0.
 *
 * @see incrementScore()
 * @see extendSnake()
 * @see generateFruit()
 */
int eatFruit() {
    int fruit_x = gameFruit.x;
    int fruit_y = gameFruit.y;
    gameBoard.grid[gameFruit.x][gameFruit.y].containsFruit = 0;
    incrementScore();
    extendSnake();
    generateFruit();
    if (fruit_x <= 2 || fruit_x >= (GAMEGRIDCOLS - 2) || fruit_y <= 2 || fruit_y >= (GAMEGRIDROWS - 2)) {
        return 1;
    }
    return 0;
}

/**
 * @brief Extends the snake by adding a new segment to its tail.
 *
 * Traverses the snake's linked list to locate the current tail node,
 * then allocates and attaches a new node behind it using the previous
 * position of the tail segment as its initial coordinates.
 *
 * This function is typically called after the snake eats a fruit.
 *
 * @see createSnakeNode()
 * @see eatFruit()
 */
void extendSnake(void) {
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

/**
 * @brief Updates the snake's position on the grid based on its current direction.
 *
 * Moves the snake head one cell in the current movement direction and updates
 * all following body segments to occupy the position previously held by the
 * segment ahead of them.
 *
 * This function also updates the game grid flags (`containsHead` and
 * `containsSnake`) to reflect the snake's new position. If the snake head
 * pointer is NULL, an error is logged.
 *
 * @note This function does not perform collision checks; call collisionCheck()
 *       separately after movement.
 *
 * @see collisionCheck()
 * @see changeSnakeDirection()
 * @see logError()
 */
void moveSnake(void) {
    if (snake.node == NULL) {
        logError(L"Error in function moveSnake() of game.h.\n\tsnake.node == NULL\n");
    }

    SnakeNode* ptr = snake.node;
    ptr->prev_x = ptr->x;
    ptr->prev_y = ptr->y;
    gameBoard.grid[ptr->x][ptr->y].containsHead = 0;

    if (snake.movement_direction == DIRECTION_UP) {
        ptr->y -= 1;
    } else if (snake.movement_direction == DIRECTION_DOWN) {
        ptr->y += 1;
    } else if (snake.movement_direction == DIRECTION_LEFT) {
        ptr->x -= 1;
    } else if (snake.movement_direction == DIRECTION_RIGHT) {
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

/**
 * @brief Updates the snake's movement direction.
 *
 * Sets the snake's global movement direction to the specified value.
 * The direction is expected to match one of the defined constants:
 * - DIRECTION_UP
 * - DIRECTION_DOWN
 * - DIRECTION_LEFT
 * - DIRECTION_RIGHT
 *
 * @param direction The new direction for the snake to move.
 */
void changeSnakeDirection(int direction) {
    snake.movement_direction = direction;
}

/**
 * @brief Increases the player's score.
 *
 * Adds SCORE_INCREMENT to the global score variable.
 * Typically called when the snake eats a fruit.
 *
 * @see score
 * @see SCORE_INCREMENT
 */
void incrementScore() {
    score += SCORE_INCREMENT;
}

/**
 * @brief Checks for collisions at the snake's current head position.
 *
 * Determines whether the snake's head has collided with a wall, its own body,
 * or a fruit, and returns a corresponding collision code.
 *
 * @return One of the following integer codes:
 * - COLLISION — if the snake hits a wall or its own body.
 * - EATS_FRUIT — if the snake moves onto a fruit.
 * - NO_COLLISION — if no collision occurs.
 *
 * @see moveSnake()
 * @see generateNextFrame()
 * @see logError()
 */
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

/**
 * @brief Retrieves the width of a single game grid cell, in pixels.
 *
 * Returns the current pixel width of each grid cell based on the
 * game board's calculated dimensions.
 *
 * @return The width of one cell in pixels.
 */
int getGameBoardCellWidth() {
    return gameBoard.cell_width;
}

/**
 * @brief Retrieves the height of a single game grid cell, in pixels.
 *
 * Returns the current pixel height of each grid cell based on the
 * game board's calculated dimensions.
 *
 * @return The height of one cell in pixels.
 */
int getGameBoardCellHeight() {
    return gameBoard.cell_height;
}

/**
 * @brief Returns a copy of the current game board rectangle.
 *
 * Produces a copy of the GameBoardRect structure describing the
 * board's pixel boundaries and dimensions. This prevents direct
 * modification of the global gameBoard data from outside code.
 *
 * @return A GameBoardRect struct containing the board's position and size.
 *
 * @see GameBoardRect
 */
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

/**
 * @brief Updates the game board's pixel dimensions and cell sizes.
 *
 * Calculates and centers the game board within the given main window
 * rectangle. It then updates the board's bounding coordinates and computes
 * the pixel width and height of each grid cell.
 *
 * Logs an error if the game board dimensions are not evenly divisible by
 * the number of grid rows or columns.
 *
 * @param mainWindowRect The RECT structure representing the dimensions of the main window.
 *
 * @see logError()
 */
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

/**
 * @brief Calculates the pixel boundaries of a specific grid cell.
 *
 * Given the grid coordinates (x, y), this function computes a RECT
 * representing that cell's pixel boundaries relative to the top-left
 * corner of the game board. This version assumes (0, 0) is the board origin.
 *
 * @param x The cell's horizontal grid index.
 * @param y The cell's vertical grid index.
 * @return A RECT structure defining the pixel boundaries of the cell.
 */
RECT getCellBoundingRect(int x, int y) {
    RECT rect;
    rect.right = x * gameBoard.cell_width;
    rect.left = rect.right - gameBoard.cell_width;
    rect.bottom = y * gameBoard.cell_height;
    rect.top = rect.bottom - gameBoard.cell_height;
    return rect;
}

RECT getNodeBoundingRect(int x, int y) {
    RECT rect;
    rect.right = ((x + 1) * gameBoard.cell_width);
    rect.left = rect.right - snake.node_diameter;
    rect.bottom = ((y + 1) * gameBoard.cell_height);
    rect.top = rect.bottom - snake.node_diameter;
    return rect;
}

/*   -------------   */

/*   --- Clean Up ---   */

/**
 * @brief Frees all dynamically allocated game data.
 *
 * Deallocates memory for the game grid and releases all associated
 * resources used by the snake. This function should be called when
 * closing the game or restarting to ensure no memory leaks occur.
 *
 * Internally, it:
 * - Frees each row of the game grid.
 * - Frees the grid pointer itself.
 * - Calls freeSnake() to deallocate the snake's linked list.
 *
 * @see freeSnake()
 */
void freeGameData() {
    for (int i = 0; i < GAMEGRIDROWS; i++) {
        free(gameBoard.grid[i]);
    }
    free(gameBoard.grid);
    freeSnake();
}

/**
 * @brief Frees all dynamically allocated snake nodes.
 *
 * Iterates through the snake's linked list, deallocating each node
 * until the entire snake structure is released. After completion,
 * the global snake head pointer is set to NULL.
 *
 * Logs an error if the snake head pointer is unexpectedly NULL
 * before freeing begins.
 *
 * @see logError()
 */
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
