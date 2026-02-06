#ifndef GAME_MACROS_H
#define GAME_MACROS_H

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

#endif