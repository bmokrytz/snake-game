#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <windows.h>

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
    BOOL boost_recharging;
} SnakeHead;

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

    SnakeHead snake;
    SnakeNode* snakeNodeIterator;

    HFONT scoreFont;
    HFONT energyFont;

    wchar_t score_label[10];
    wchar_t score_text[20];

    int energy_level;
} GameBoard;

#endif