#ifndef GAME_H
#define GAME_H

#include <windows.h>  // required because GameBoard exposes HWND/HFONT/RECT and API uses HWND

// ==================== Public Constants ====================

// Board / grid
#define GAMEBOARDWIDTH   600
#define GAMEBOARDHEIGHT  600
#define GAMEGRIDROWS     60
#define GAMEGRIDCOLS     60

#define SNAKEHEADSTARTX  (GAMEGRIDCOLS / 2)
#define SNAKEHEADSTARTY  (GAMEGRIDROWS / 2)

// Directions
#define DIRECTION_UP     0
#define DIRECTION_DOWN   1
#define DIRECTION_LEFT   2
#define DIRECTION_RIGHT  3

// Events
#define EATS_FRUIT       4
#define COLLISION        5
#define NO_COLLISION     6

#define COLLISION_RANGE  1
#define FRUIT_MARGIN     3

// Game states
#define PAUSE_GAME       7
#define START_GAME       8
#define GAME_OVER        9

// Energy
#define ENERGY_DECAY_VALUE      4
#define ENERGY_REPLENISH_VALUE  4

// Timers
#define GAME_TIMER_NORMAL_SPEED_ID           1
#define GAME_TIMER_NORMAL_SPEED_VAL          60

#define GAME_TIMER_SLOW_SPEED_ID             2
#define GAME_TIMER_SLOW_SPEED_VAL            80

#define GAME_TIMER_FAST_SPEED_ID             3
#define GAME_TIMER_FAST_SPEED_VAL            45

#define GAME_TIMER_BOOST_ID                  4
#define GAME_TIMER_BOOST_VAL                 20

#define GAME_TIMER_BOOST_RECHARGE_ID         5
#define GAME_TIMER_BOOST_RECHARGE_VAL        400

// ==================== Types ====================

typedef struct Coord {
    int x;
    int y;
} Coord;

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

    int score;
    int score_increment;
    BOOL update_score;

    int gameStatus;
    int game_timer_id;

    Coord fruitLoc;

    HFONT scoreFont;
    HFONT energyFont;

    wchar_t score_label[10];
    wchar_t score_text[20];

    int energy_level;
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

    int node_diameter;

    BOOL boost;
    BOOL boost_depleted;
    BOOL boost_recharging;
} SnakeHead;

// ==================== Globals ====================
// These must be *defined* in exactly one .c file (game.c), not in the header.
extern GameBoard gameBoard;
extern SnakeHead snake;

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

void setBoost(HWND hwnd);
void disableBoost(HWND hwnd);
void setBoostDepleted(HWND hwnd);

void updateEnergyLevel(HWND hwnd);
void startBoostRecharge(HWND hwnd);
void stopBoostRecharge(HWND hwnd);

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

RECT getCellBoundingRect(int x, int y);
RECT getNodeBoundingRect(int x, int y);
RECT getNodeInvalidationRect(int x, int y);

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
