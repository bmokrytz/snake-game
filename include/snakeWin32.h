/*
 * snakeWin32.h
 * -----------
 * Public interface for the Win32 windowing and rendering module.
 * Declares window setup, message handling, and drawing helpers used by platform.c.
 *
 * No game rules/state live here (those are in game.c).
 */


#ifndef WINDOW_H
#define WINDOW_H

// Ensure Windows APIs use Unicode versions
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

// ==================== Public Constants ====================

// Window classes
#define MAIN_WINDOW_CLASS L"MAIN_WIN"
#define MENU_WINDOW_CLASS L"MENU_WIN"
#define GAME_WINDOW_CLASS L"SNAKE_WIN"

// Brush colors
#define COLOR_SNAKEGAME_UI_TEXT RGB(32, 42, 49)
#define COLOR_SNAKEGAME_BACKGROUND RGB(32, 42, 49)
#define COLOR_SNAKEGAME_GAMEFIELD RGB(245, 245, 245)
#define COLOR_SNAKEGAME_SNAKE RGB(30, 200, 70)
#define COLOR_SNAKEGAME_FRUIT RGB(255, 140, 0)
#define COLOR_SNAKEGAME_WALL RGB(0, 0, 0)

// Display modes
#define DISPLAY_MODE_WINDOWED       9000
#define DISPLAY_MODE_BORDERLESS     9001

// Button sizing / constraints
#define MAX_BUTTONS 32
#define BUTTON_WIDTH_MAIN               100
#define BUTTON_HEIGHT_MAIN              30
#define BUTTON_WIDTH_MENU               130
#define BUTTON_HEIGHT_MENU              30
#define BUTTON_WIDTH_GAME_RESET         130
#define BUTTON_HEIGHT_GAME_RESET        30

#define BUTTON_WIDTH_GAME_CONTAINER     100
#define BUTTON_HEIGHT_GAME_CONTAINER    30

// Button IDs / command IDs
#define ID_DEBUG_1      10001
#define ID_WINDOWED     10002
#define ID_BORDERLESS   10003
#define ID_START_GAME   10004
#define ID_RESET_GAME   10005

// Directions
#define DIRECTION_UP     0
#define DIRECTION_DOWN   1
#define DIRECTION_LEFT   2
#define DIRECTION_RIGHT  3

// ==================== Types ====================

typedef struct BrushHandler {
    HBRUSH backgroundBrush;
    HBRUSH wallBrush;
    HBRUSH fieldBrush;
    HBRUSH snakeBrush;
    HBRUSH fruitBrush;
} BrushHandler;

typedef struct ButtonList {
    HWND handles[MAX_BUTTONS];
    int count;
} ButtonList;

typedef struct ButtonConfig {
    wchar_t label[30];
    HWND parent;
    int ID;
    int x;
    int y;
    int width;
    int height;
} ButtonConfig;

typedef struct WindowRECT {
    int left;
    int top;
    int right;
    int bottom;
    int width;
    int height;
} WindowRECT;

typedef struct WindowHandler {
    HWND mainWindow;
    HWND menuWindow;
    HWND gameContainerWindow;
    HWND gameDataDisplayWindow;
    HWND gameFieldWindow;
    HWND gameEnergyWindow;
    WindowRECT windowedConfig;
    WindowRECT borderlessConfig;
    WindowRECT minWindowedConfig;
    int displayMode;
} WindowHandler;

typedef struct AnimationHandler {
    BOOL animate;
    BOOL gameOver;
    int gameOver_ttl;
} AnimationHandler;

typedef struct WindowState {
    HDC     staticDC;
    HBITMAP staticBmp;
    HBITMAP staticOldBmp;

    HDC     frameDC;
    HBITMAP frameBmp;
    HBITMAP frameOldBmp;

    int     width;
    int     height;

    BOOL    staticDirty;
} WindowState;

typedef void (*onWindowCloseCallbackFn)(void);

/*==============================================================================
 *                           GLOBAL  VARIABLES
 *============================================================================*/

extern WindowHandler windowHandler;
extern BrushHandler brushHandler;
extern AnimationHandler animationHandler;
extern int counter;


/*==============================================================================
 *                           FUNCTION  PROTOTYPES
 *============================================================================*/

/** @defgroup WindowFunctions Window Management
 *  @brief Functions that handle window creation, events, and cleanup.
 *  @{
 */

/*----------------------------------------------------------------------------*/
/*                             Window Initialization                          */
/*----------------------------------------------------------------------------*/

void windowSetup(HINSTANCE hInstance);
void initializeWindowHandler();
void initializeAnimationHandler();
void loadSingleFont(LPCWSTR name, DWORD  fl, PVOID  res);
void loadFonts();
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t *className, WNDPROC windowProc);
void setWindowConfigs();
void buildWindows(HINSTANCE hInstance);
void buildMainWindow(HINSTANCE hInstance);
void buildMenuWindow(HINSTANCE hInstance);
void buildGameWindows(HINSTANCE hInstance);
void buildGameContainerWindow(HINSTANCE hInstance);
void buildGameDataDisplayWindow(HINSTANCE hInstance);
void buildGameFieldWindow(HINSTANCE hInstance);
void buildGameEnergyWindow(HINSTANCE hInstance);
WindowRECT getSnakeWindowRect(HWND hwnd);
WindowRECT getMainWindowRect();
WindowRECT getMenuWindowRect();
WindowRECT getGameContainerWindowRect();
WindowRECT getGameDataDisplayWindowRect();
WindowRECT getGameFieldWindowRect();
WindowRECT getGameEnergyWindowRect();
HWND createButton(ButtonConfig config);
void initializeBrushes();
void win32SetOnCloseCallback(onWindowCloseCallbackFn fn);

/*----------------------------------------------------------------------------*/
/*                             Window Procedure                               */
/*----------------------------------------------------------------------------*/

LRESULT CALLBACK SnakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*----------------------------------------------------------------------------*/
/*                             Window Utility                                 */
/*----------------------------------------------------------------------------*/

void updateDisplayMode();
void switchWindows(HWND hwnd_current, HWND hwnd_next);
void resizeMenuWindow();
void resizeGameContainerWindow();
void resizeGameDataDisplayWindow();
void resizeGameFieldWindow();
void resizeGameEnergyWindow();
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);
ButtonList getButtonList(HWND parent);
RECT getEnergyMeterOuterRect();


/*----------------------------------------------------------------------------*/
/*                             Window Graphics                                */
/*----------------------------------------------------------------------------*/

void paintMainWindow();
void paintMenuWindow();
void paintGameContainerWindow();
void PaintGameDataDisplayWindow();
void paintGameFieldWindow();
void paintGameEnergyWindow();
void drawEnergyDisplay(HDC hdc, RECT innerRect, int innerHeight);
void drawDebugGrid(RECT field, HDC hdc);
void drawGameField(RECT field, HDC hdc);
void drawSnake(HDC hdc);
void drawFruit(HDC hdc);
void drawWalls(HDC hdc);
void drawCircle(HDC hdc, RECT cell_bounds);

/*----------------------------------------------------------------------------*/
/*                              Window Cleanup                                */
/*----------------------------------------------------------------------------*/

void windowCleanUp(void);
void deleteBrushes(void);


// TEMPORARY
void animation_game_over(HDC hdc);

void DrawOutlinedText(
    HDC hdc,
    LPCWSTR text,
    RECT* rc,
    COLORREF textColor,
    COLORREF outlineColor,
    int outlinePx,
    UINT format
);


#endif
