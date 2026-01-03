/**
 * @file window.h
 * @author Brandon Mokrytzki
 * @date 07-OCT-2025
 * @brief Handles all drawing and graphical rendering for the Snake game.
 *
 * This module manages all GDI-based drawing routines including the game field,
 * snake, fruit, and debug grid. It interfaces with the Windows API through
 * device contexts (HDC) and brushes.
 *
 * Responsibilities:
 * - Drawing the main window and game window contents
 * - Rendering walls, snake segments, and background
 * - Managing drawing brushes and debug overlays
 *
 * @see <windows.h>
 * @see game.h
 * @see snake.c
 */
#ifndef WINDOW_H
#define WINDOW_H

// Ensure Windows APIs use Unicode versions
#ifndef UNICODE
#define UNICODE
#endif

/*==============================================================================
 *                           INCLUDES
 *============================================================================*/

#include <windows.h>
#include "game.h"

/*==============================================================================
 *                           MACROS
 *============================================================================*/

#ifndef WIN_MACROS
#define WIN_MACROS
#define MAIN_WINDOW_CLASS L"MAIN_WIN"
#define MENU_WINDOW_CLASS L"MENU_WIN"
#define GAME_WINDOW_CLASS L"SNAKE_WIN"

#define COLOR_SNAKEGAME_UI_TEXT RGB(32, 42, 49)
#define COLOR_SNAKEGAME_BACKGROUND RGB(32, 42, 49)
#define COLOR_SNAKEGAME_GAMEFIELD RGB(245, 245, 245)
#define COLOR_SNAKEGAME_SNAKE RGB(30, 200, 70)
#define COLOR_SNAKEGAME_FRUIT RGB(255, 140, 0)
#define COLOR_SNAKEGAME_WALL RGB(0, 0, 0)

#define DISPLAY_MODE_WINDOWED       9000
#define DISPLAY_MODE_BORDERLESS     9001

#define MAX_BUTTONS 32
#define BUTTON_WIDTH_MAIN               100
#define BUTTON_HEIGHT_MAIN              30
#define BUTTON_WIDTH_MENU               130
#define BUTTON_HEIGHT_MENU              30
#define BUTTON_WIDTH_GAME_RESET         130
#define BUTTON_HEIGHT_GAME_RESET        30

#define BUTTON_WIDTH_GAME_CONTAINER     100
#define BUTTON_HEIGHT_GAME_CONTAINER    30

#define ID_DEBUG_1      10001
#define ID_WINDOWED     10002
#define ID_BORDERLESS   10003
#define ID_START_GAME   10004
#define ID_RESET_GAME   10005

// Timer macros
#define NORMAL_TICK_SPEED_TIMER_ID 1
#define NORMAL_TICK_SPEED_TIMER_VAL 45
#define SLOW_TICK_SPEED_TIMER_ID 2
#define SLOW_TICK_SPEED_TIMER_ID 45
#define FAST_TICK_SPEED_TIMER_ID 3
#define FAST_TICK_SPEED_TIMER_ID 45
#define ANIMATION_TIMER_ID 4
#define ANIMATION_TIMER_VAL 45

#endif


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



/*==============================================================================
 *                           GLOBAL  VARIABLES
 *============================================================================*/

WindowHandler windowHandler;

BrushHandler brushHandler;

AnimationHandler animationHandler;

int counter = 0;


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
static void repaintAllWindows();
static void resizeAllWindowBackbuffers(HWND hwnd);
static void resizeWindowBackbuffer(HWND hwnd);
HWND createButton(ButtonConfig config);
void initializeBrushes();

/*----------------------------------------------------------------------------*/
/*                             Window Procedure                               */
/*----------------------------------------------------------------------------*/

LRESULT CALLBACK SnakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*----------------------------------------------------------------------------*/
/*                             Window Utility                                 */
/*----------------------------------------------------------------------------*/

void updateGameboardPos();
void updateDisplayMode();
void switchWindows(HWND hwnd_current, HWND hwnd_next);
static void resizeAllWindows();
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

void debugDropWindowedConfig();
void debugDropMainWindowSizePosition();
void debugLogRECT(RECT rect);
void debugLogWindowRECT(WindowRECT windowRect);

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










/** @} */  // end of WindowFunctions

/*==============================================================================
 *                           FUNCTION  IMPLEMENTATIONS
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*                              Setup                                         */
/*----------------------------------------------------------------------------*/

/**
 * @brief Initializes all window-related resources for the Snake game.
 *
 * Registers the main and game window classes, creates both windows,
 * and initializes rendering brushes. This function should be called
 * once during program startup, before the main message loop begins.
 *
 * Internally, it:
 * - Registers window classes for the main and game windows.
 * - Creates the main application window and its embedded child window.
 * - Initializes GDI brushes used for rendering.
 *
 * @param hInstance Handle to the current application instance (provided by WinMain).
 *
 * @see RegisterWindowClass()
 * @see buildWindows()
 * @see initializeBrushes()
 */
void windowSetup(HINSTANCE hInstance) {
    loadFonts();
    initializeBrushes();
    initializeWindowHandler();
    initializeAnimationHandler();
    setWindowConfigs();
    RegisterWindowClass(hInstance, MAIN_WINDOW_CLASS, SnakeWindowProc);
    RegisterWindowClass(hInstance, MENU_WINDOW_CLASS, SnakeWindowProc);
    RegisterWindowClass(hInstance, GAME_WINDOW_CLASS, SnakeWindowProc);
    buildWindows(hInstance);
}

void initializeWindowHandler() {
    windowHandler.mainWindow = NULL;            windowHandler.menuWindow = NULL;
    windowHandler.gameContainerWindow = NULL;   windowHandler.gameFieldWindow = NULL;
    windowHandler.gameDataDisplayWindow = NULL; windowHandler.gameEnergyWindow = NULL;
    windowHandler.displayMode = DISPLAY_MODE_WINDOWED;
}

void initializeAnimationHandler() {
    animationHandler.animate = FALSE;
    animationHandler.gameOver = FALSE;
    animationHandler.gameOver_ttl = 0;
}

void loadFonts() {
    int fontsLoaded = AddFontResourceEx(L"assets\\font\\Jersey_25\\Jersey25-Regular.ttf", FR_PRIVATE, NULL);
    if (fontsLoaded == 0) logError(L"Error loading font in windowSetup().\n\tFont loading failed.\n");
    fontsLoaded = AddFontResourceEx(L"assets\\font\\DM_Serif_Text\\DMSerifText-Regular.ttf", FR_PRIVATE, NULL);
    if (fontsLoaded == 0) logError(L"Error loading font in windowSetup().\n\tFont loading failed.\n");
    fontsLoaded = AddFontResourceEx(L"assets\\font\\DM_Serif_Text\\DMSerifText-Italic.ttf", FR_PRIVATE, NULL);
    if (fontsLoaded == 0) logError(L"Error loading font in windowSetup().\n\tFont loading failed.\n");
    gameBoard.scoreFont = CreateFont(
        64,                 // height (font size in logical units)
        0,                  // width (0 = auto)
        0,                  // escapement
        0,                  // orientation
        FW_NORMAL,          // weight (FW_BOLD, FW_LIGHT, etc.)
        FALSE,              // italic
        FALSE,              // underline
        FALSE,              // strikeout
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        VARIABLE_PITCH,
        TEXT("Jersey 25")
    );
    if (gameBoard.scoreFont == NULL) {
        logError(L"Error creating font (gameBoard.scoreFont) in windowSetup().\n\tFont creation failed.\n");
    }

    gameBoard.energyFont = CreateFont(
        24,                 // height (font size in logical units)
        0,                  // width (0 = auto)
        0,                  // escapement
        0,                  // orientation
        FW_NORMAL,          // weight (FW_BOLD, FW_LIGHT, etc.)
        FALSE,              // italic
        FALSE,              // underline
        FALSE,              // strikeout
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        VARIABLE_PITCH,
        TEXT("Jersey 25")
    );
    if (gameBoard.energyFont == NULL) {
        logError(L"Error creating font (gameBoard.energyFont) in windowSetup().\n\tFont creation failed.\n");
    }
}

/**
 * @brief Registers a custom window class with the Windows API.
 *
 * Defines the properties of a new window class, including its
 * window procedure, instance handle, cursor, and class name.
 * This function must be called before creating any windows
 * using that class.
 *
 * @param hInstance  Handle to the current application instance.
 * @param className  The unique name for the window class to register.
 * @param windowProc The callback function that will handle window messages.
 *
 * @see SnakeWindowProc()
 * @see buildWindows()
 */
void RegisterWindowClass (HINSTANCE hInstance, const wchar_t * className, WNDPROC windowProc) {
    WNDCLASS wc = { };

    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);
}

void setWindowConfigs() {
    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    windowHandler.borderlessConfig.width = screenWidth;
    windowHandler.borderlessConfig.height = screenHeight;
    windowHandler.borderlessConfig.left = 0;
    windowHandler.borderlessConfig.top = 0;
    windowHandler.borderlessConfig.right = windowHandler.borderlessConfig.left + windowHandler.borderlessConfig.width;
    windowHandler.borderlessConfig.bottom = windowHandler.borderlessConfig.top + windowHandler.borderlessConfig.height;

    windowHandler.minWindowedConfig.width = GAMEBOARDWIDTH + 300;
    windowHandler.minWindowedConfig.height = GAMEBOARDHEIGHT + 600;
    windowHandler.minWindowedConfig.left = (screenWidth / 2) - (windowHandler.minWindowedConfig.width / 2);
    windowHandler.minWindowedConfig.top = (screenHeight / 2) - (windowHandler.minWindowedConfig.height / 2);
    windowHandler.minWindowedConfig.right = windowHandler.minWindowedConfig.left + windowHandler.minWindowedConfig.width;
    windowHandler.minWindowedConfig.bottom = windowHandler.minWindowedConfig.top + windowHandler.minWindowedConfig.height;

    windowHandler.windowedConfig.width = windowHandler.minWindowedConfig.width;
    windowHandler.windowedConfig.height = windowHandler.minWindowedConfig.height;
    windowHandler.windowedConfig.left = windowHandler.minWindowedConfig.left;
    windowHandler.windowedConfig.top = windowHandler.minWindowedConfig.top;
    windowHandler.windowedConfig.right = windowHandler.windowedConfig.left + windowHandler.windowedConfig.width;
    windowHandler.windowedConfig.bottom = windowHandler.windowedConfig.top + windowHandler.windowedConfig.height;
}

/**
 * @brief Creates the main application window and the embedded game window.
 *
 * This function creates two Win32 windows:
 * - The **main window** (WS_OVERLAPPEDWINDOW), which contains the title bar
 *   and acts as the parent container.
 * - The **game window** (WS_CHILD | WS_VISIBLE), which displays the game grid.
 *
 * After window creation, the game board’s position is updated and aligned
 * within the parent window’s client area.
 *
 * Logs an error if either window fails to create.
 *
 * @param hInstance Handle to the current application instance.
 *
 * @see logError()
 * @see updateGameboardPos()
 * @see getGameboardRect()
 */
void buildWindows(HINSTANCE hInstance) {

    buildMainWindow(hInstance);
    if (windowHandler.mainWindow == NULL) {
        logError(L"Error in function buildWindows() of window.h.\n\tmainWindow == NULL. Window creation failed.\n");
    }
    updateGameboardPos();
    initializeCellAndNodeData();
    buildMenuWindow(hInstance);
    if (windowHandler.menuWindow == NULL) {
        logError(L"Error in function buildWindows() of window.h.\n\tmenuWindow == NULL. Window creation failed.\n");
    }
    buildGameWindows(hInstance);
    ShowWindow(windowHandler.mainWindow, SW_SHOW);
    ShowWindow(windowHandler.menuWindow, SW_SHOW);
    ShowWindow(windowHandler.gameContainerWindow, SW_HIDE);
    ShowWindow(windowHandler.gameDataDisplayWindow, SW_HIDE);
    ShowWindow(windowHandler.gameFieldWindow, SW_HIDE);
    ShowWindow(windowHandler.gameEnergyWindow, SW_HIDE);
    SetFocus(windowHandler.menuWindow);
}




void buildMainWindow(HINSTANCE hInstance) {
    WindowRECT mainWinRect = getMainWindowRect();
    DWORD winStyle;
    if (windowHandler.displayMode == DISPLAY_MODE_BORDERLESS) winStyle = WS_POPUP | WS_VISIBLE;
    else winStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    windowHandler.mainWindow = CreateWindowEx(
        0,                              // Optional window styles.
        MAIN_WINDOW_CLASS,                      // Window class
        L"SNAKE",                       // Window text
        winStyle,            // Window style
        // Size and position
        mainWinRect.left, mainWinRect.top, 
        mainWinRect.width, mainWinRect.height,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

WindowRECT getSnakeWindowRect(HWND hwnd) {
    if (hwnd == windowHandler.mainWindow) {
        return getMainWindowRect();
    }
    else if (hwnd == windowHandler.menuWindow) {
        return getMenuWindowRect();
    }
    else if (hwnd == windowHandler.gameContainerWindow) {
        return getGameContainerWindowRect();
    }
    else if (hwnd == windowHandler.gameDataDisplayWindow) {
        return getGameDataDisplayWindowRect();
    }
    else if (hwnd == windowHandler.gameFieldWindow) {
        return getGameFieldWindowRect();
    }
    else if (hwnd == windowHandler.gameEnergyWindow) {
        return getGameEnergyWindowRect();
    }
    else {
        return getMainWindowRect();
        /*
        wchar_t msg[900];
        swprintf(msg, 900, L"In getSnakeWindowRect():\n\tmainWindow HWND = %p\n", (void*)windowHandler.mainWindow);
        swprintf(msg, 900, L"%s\tmenuWindow HWND = %p\n", msg, (void*)windowHandler.menuWindow);
        swprintf(msg, 900, L"%s\tgameContainerWindow HWND = %p\n", msg, (void*)windowHandler.gameContainerWindow);
        swprintf(msg, 900, L"%s\tgameDataDisplayWindow HWND = %p\n", msg, (void*)windowHandler.gameDataDisplayWindow);
        swprintf(msg, 900, L"%s\tgameFieldWindow HWND = %p\n", msg, (void*)windowHandler.gameFieldWindow);
        swprintf(msg, 900, L"%s\tgetSnakeWindowRect was passed HWND = %p\n\n", msg, (void*)hwnd);
        logError(msg);
        */
    }
}

WindowRECT getMainWindowRect() {
    if (windowHandler.displayMode == DISPLAY_MODE_BORDERLESS) {
        return windowHandler.borderlessConfig;
    }
    else {
        return windowHandler.windowedConfig;
    }
}

void buildMenuWindow(HINSTANCE hInstance) {
    WindowRECT menuWindowRect = getMenuWindowRect();
    windowHandler.menuWindow = CreateWindowEx(
        0,                              // Optional window styles.
        MENU_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        menuWindowRect.left, menuWindowRect.top, 
        menuWindowRect.width, menuWindowRect.height,
        windowHandler.mainWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

WindowRECT getMenuWindowRect() {
    RECT mainRect; GetClientRect(windowHandler.mainWindow, &mainRect);
    WindowRECT menuWindowRect = {
        .left = mainRect.left,
        .top = mainRect.top,
        .right = mainRect.right,
        .bottom = mainRect.bottom,
        .width = mainRect.right - mainRect.left,
        .height = mainRect.bottom - mainRect.top
    };
    return menuWindowRect;
}

void buildGameWindows(HINSTANCE hInstance) {
    buildGameContainerWindow(hInstance);
    if (windowHandler.gameContainerWindow == NULL) {
        logError(L"Error in function buildGameWindows() of window.h.\n\tgameContainerWindow == NULL. Window creation failed.\n");
    }
    buildGameDataDisplayWindow(hInstance);
    if (windowHandler.gameDataDisplayWindow == NULL) {
        logError(L"Error in function buildGameWindows() of window.h.\n\tgameDataDisplayWindow == NULL. Window creation failed.\n");
    }
    buildGameFieldWindow(hInstance);
    if (windowHandler.gameFieldWindow == NULL) {
        logError(L"Error in function buildGameWindows() of window.h.\n\tgameFieldWindow == NULL. Window creation failed.\n");
    }
    buildGameEnergyWindow(hInstance);
    if (windowHandler.gameEnergyWindow == NULL) {
        logError(L"Error in function buildGameWindows() of window.h.\n\tgameEnergyWindow == NULL. Window creation failed.\n");
    }
}

void buildGameContainerWindow(HINSTANCE hInstance) {
    WindowRECT gameContainerWindowRect = getGameContainerWindowRect();
    windowHandler.gameContainerWindow = CreateWindowEx(
        0,                              // Optional window styles.
        MENU_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        gameContainerWindowRect.left, gameContainerWindowRect.top, 
        gameContainerWindowRect.width, gameContainerWindowRect.height,
        windowHandler.mainWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

WindowRECT getGameContainerWindowRect() {
    RECT mainRect; GetClientRect(windowHandler.mainWindow, &mainRect);
    WindowRECT gameContainerWindowRect = {
        .left = mainRect.left,
        .top = mainRect.top,
        .right = mainRect.right,
        .bottom = mainRect.bottom,
        .width = mainRect.right - mainRect.left,
        .height = mainRect.bottom - mainRect.top
    };
    return gameContainerWindowRect;
}

void buildGameDataDisplayWindow(HINSTANCE hInstance) {
    WindowRECT gameDataDisplayWindowRect = getGameDataDisplayWindowRect();

    windowHandler.gameDataDisplayWindow = CreateWindowEx(
        0,                              // Optional window styles.
        GAME_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        gameDataDisplayWindowRect.left, gameDataDisplayWindowRect.top, 
        gameDataDisplayWindowRect.width, gameDataDisplayWindowRect.height,
        windowHandler.gameContainerWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

WindowRECT getGameDataDisplayWindowRect() {
    WindowRECT gameContainerWindowRect = getGameContainerWindowRect();
    int windowMargin_horiz = gameContainerWindowRect.width / 3;
    int windowMargin_vert = 10;
    int windowWidth = windowMargin_horiz;
    int windowHeight = gameContainerWindowRect.height / 8;

    WindowRECT gameDataDisplayWindowRect = {
        .left = gameContainerWindowRect.left + windowMargin_horiz,
        .top = gameContainerWindowRect.top + windowMargin_vert,
        .right = (gameContainerWindowRect.left + windowMargin_horiz) + windowWidth,
        .bottom = (gameContainerWindowRect.top + windowMargin_vert) + windowHeight,
        .width = windowWidth,
        .height = windowHeight
    };
    return gameDataDisplayWindowRect;
}

void buildGameFieldWindow(HINSTANCE hInstance) {
    WindowRECT gameFieldWindowRect = getGameFieldWindowRect();
    windowHandler.gameFieldWindow = CreateWindowEx(
        0,                              // Optional window styles.
        GAME_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        gameFieldWindowRect.left, gameFieldWindowRect.top, 
        gameFieldWindowRect.width, gameFieldWindowRect.height,
        windowHandler.gameContainerWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

WindowRECT getGameFieldWindowRect() {
    //RECT mainRect; GetClientRect(windowHandler.mainWindow, &mainRect);
    //updateGameboard(mainRect);
    GameBoardRect gameboardRect = getGameboardRect();

    WindowRECT gameFieldWindowRect = {
        .left = gameboardRect.left,
        .top = gameboardRect.top,
        .right = gameboardRect.left + gameboardRect.width,
        .bottom = gameboardRect.top + gameboardRect.height,
        .width = gameboardRect.width,
        .height = gameboardRect.height
    };
    return gameFieldWindowRect;
}

void buildGameEnergyWindow(HINSTANCE hInstance) {
    WindowRECT gameEnergyWindowRect = getGameEnergyWindowRect();
    windowHandler.gameEnergyWindow = CreateWindowEx(
        0,                              // Optional window styles.
        GAME_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        gameEnergyWindowRect.left, gameEnergyWindowRect.top, 
        gameEnergyWindowRect.width, gameEnergyWindowRect.height,
        windowHandler.gameContainerWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

WindowRECT getGameEnergyWindowRect() {
    RECT containerRect; GetClientRect(windowHandler.gameContainerWindow, &containerRect);
    WindowRECT gameFieldWindowRect = getGameFieldWindowRect();
    int gameFieldHeight = gameFieldWindowRect.bottom - gameFieldWindowRect.top;
    //int energyWindowHeight = (gameFieldHeight / 3) + 50;
    int energyWindowHeight = (gameFieldHeight / 3);
    //int energyWindowWidth = 70;
    int energyWindowWidth = 70;
    int energyMeterWallThickness = 10;
    int padding_right = 20;
    int right = (gameFieldWindowRect.left - padding_right);
    int left = right - energyWindowWidth;
    int top = (gameFieldWindowRect.top + (gameFieldHeight / 2)) - (energyWindowHeight / 2);
    int bottom = top + energyWindowHeight;

    WindowRECT gameEnergyWindowRect = {
        .left = left,
        .top = top,
        .right = right,
        .bottom = bottom,
        .width = energyWindowWidth,
        .height = energyWindowHeight
    };
    return gameEnergyWindowRect;
}

/*
WindowRECT getGameEnergyWindowRect() {
    RECT containerRect; GetClientRect(windowHandler.gameContainerWindow, &containerRect);
    RECT gameFieldRect; GetClientRect(windowHandler.gameFieldWindow, &gameFieldRect);
    WindowRECT gameFieldWindowRect = getGameFieldWindowRect();
    wchar_t msg[900];
    swprintf(msg, 900, L"getGameEnergyWindowRect Debug:\n\n");
    swprintf(msg, 900, L"%scontainerRect:\n", msg);
    logDebugMessage(msg);
    debugLogRECT(containerRect);
    swprintf(msg, 900, L"gameFieldWindowRect:\n");
    logDebugMessage(msg);
    debugLogWindowRECT(gameFieldWindowRect);
    int gameFieldHeight = gameFieldRect.bottom - gameFieldRect.top;
    int energyWindowHeight = (gameFieldHeight / 3);
    int energyWindowWidth = 50;
    int energyMeterWallThickness = 10;
    int padding_right = 20;
    int right = (gameFieldRect.left - padding_right);
    int left = right - energyWindowWidth;
    int top = (gameFieldRect.top + (gameFieldHeight / 2)) - (energyWindowHeight / 2);
    int bottom = top + energyWindowHeight;

    WindowRECT gameEnergyWindowRect = {
        .left = left,
        .top = top,
        .right = right,
        .bottom = bottom,
        .width = energyWindowWidth,
        .height = energyWindowHeight
    };
    swprintf(msg, 900, L"gameEnergyWindowRect:\n");
    logDebugMessage(msg);
    debugLogWindowRECT(gameEnergyWindowRect);
    return gameEnergyWindowRect;
}
*/

HWND createButton(ButtonConfig config) {
    return CreateWindowW(
        L"BUTTON",               // Predefined class name
        config.label,            // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        config.x, config.y, config.width, config.height,         // Position and size (x, y, width, height)
        config.parent,                    // Parent window handle
        (HMENU)config.ID,       // Control ID
        (HINSTANCE)GetWindowLongPtr(config.parent, GWLP_HINSTANCE),
        NULL);
}

void debugDropWindowedConfig() {
    wchar_t msg[700];
    swprintf(msg, 700, L"Window Config:\n");
    swprintf(msg, 700, L"%swindowHandler.windowedConfig.left = %d. windowHandler.windowedConfig.top = %d.\n", msg, windowHandler.windowedConfig.left, windowHandler.windowedConfig.top);
    swprintf(msg, 700, L"%swindowHandler.windowedConfig.width = %d. windowHandler.windowedConfig.height = %d.\n\n", msg, windowHandler.windowedConfig.width, windowHandler.windowedConfig.height);
    logDebugMessage(msg);
}

void debugDropMainWindowSizePosition() {
    wchar_t msg[700];
    WindowRECT mainWindowRect = getMainWindowRect();
    swprintf(msg, 700, L"Main Window:\n");
    swprintf(msg, 700, L"%smainWindowRect.left = %d. mainWindowRect.top = %d.\n", msg, mainWindowRect.left, mainWindowRect.top);
    swprintf(msg, 700, L"%smainWindowRect.right = %d. mainWindowRect.bottom = %d.\n", msg, mainWindowRect.right, mainWindowRect.bottom);
    swprintf(msg, 700, L"%smainWindowRect.width = %d. mainWindowRect.height = %d.\n\n", msg, mainWindowRect.width, mainWindowRect.height);
    logDebugMessage(msg);
}

void debugLogRECT(RECT rect) {
    wchar_t msg[700];
    swprintf(msg, 700, L"\tRECT:\n");
    swprintf(msg, 700, L"%s\t\trect.left = %d. rect.top = %d.\n", msg, rect.left, rect.top);
    swprintf(msg, 700, L"%s\t\trect.right = %d. rect.bottom = %d.\n", msg, rect.right, rect.bottom);
    logDebugMessage(msg);
}

void debugLogWindowRECT(WindowRECT windowRect) {
    wchar_t msg[700];
    swprintf(msg, 700, L"\twindowRECT:\n");
    swprintf(msg, 700, L"%s\t\twindowRect.left = %d. windowRect.top = %d.\n", msg, windowRect.left, windowRect.top);
    swprintf(msg, 700, L"%s\t\twindowRect.right = %d. windowRect.bottom = %d.\n", msg, windowRect.right, windowRect.bottom);
    swprintf(msg, 700, L"%s\t\twindowRect.width = %d. windowRect.height = %d.\n", msg, windowRect.width, windowRect.height);
    logDebugMessage(msg);
}

/**
 * @brief Main window procedure for handling Win32 messages.
 *
 * Processes all system and user-generated events for both the main window
 * and the embedded game window. This function handles painting, keyboard
 * input, resizing, and timer-based frame updates. It also performs cleanup
 * on application shutdown.
 *
 * #### Message handling:
 * - **WM_DESTROY** — Cleans up all resources and posts a quit message.
 * - **WM_SETCURSOR** — Restores the default cursor behavior.
 * - **WM_PAINT** — Draws the main or game window depending on which handle triggered the event.
 * - **WM_KEYDOWN** — Handles player input for snake movement (WASD or arrow keys) and pause toggling.
 * - **WM_TIMER** — Advances the game state and triggers redraws when the game is running.
 * - **WM_SIZE** — Updates window and game board layout when the window is resized.
 *
 * @param hwnd   Handle to the window receiving the message.
 * @param uMsg   The message identifier (e.g., WM_PAINT, WM_KEYDOWN, etc.).
 * @param wParam Additional message information (message-dependent).
 * @param lParam Additional message information (message-dependent).
 *
 * @return The result of message processing, as required by the Win32 API.
 *
 * @note Unhandled messages are passed to DefWindowProc() for default processing.
 *
 * @see paintMainWindow()
 * @see paintGameWindow()
 * @see generateNextFrame()
 * @see updateGameboardPos()
 * @see freeGameData()
 */
LRESULT CALLBACK SnakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            windowCleanUp();
            freeGameData();
            PostQuitMessage(0);
            return 0;
        }
        case WM_CREATE:
        {
            if (windowHandler.mainWindow == NULL) {
                createButton(
                    (ButtonConfig){
                        .label = L"Debug",
                        .parent = hwnd,
                        .ID = ID_DEBUG_1,
                        .x = 20,
                        .y = 20,
                        .width = BUTTON_WIDTH_MAIN,
                        .height = BUTTON_HEIGHT_MAIN
                    }
                );
            }
            if (windowHandler.mainWindow != NULL && windowHandler.menuWindow == NULL) {
                RECT mainRect; GetClientRect(windowHandler.mainWindow, &mainRect);
                createButton(
                    (ButtonConfig){
                        .label = L"Start Game",
                        .parent = hwnd,
                        .ID = ID_START_GAME,
                        .x = ((mainRect.right - mainRect.left) / 2) - 50,
                        .y = ((mainRect.bottom - mainRect.top) / 3),
                        .width = BUTTON_WIDTH_MENU,
                        .height = BUTTON_HEIGHT_MENU
                    }
                );
                createButton(
                    (ButtonConfig){
                        .label = L"Windowed Mode",
                        .parent = hwnd,
                        .ID = ID_WINDOWED,
                        .x = ((mainRect.right - mainRect.left) / 2) - 50,
                        .y = ((mainRect.bottom - mainRect.top) / 3) + 60,
                        .width = BUTTON_WIDTH_MENU,
                        .height = BUTTON_HEIGHT_MENU
                    }
                );
                createButton(
                    (ButtonConfig){
                        .label = L"Borderless Mode",
                        .parent = hwnd,
                        .ID = ID_BORDERLESS,
                        .x = ((mainRect.right - mainRect.left) / 2) - 50,
                        .y = ((mainRect.bottom - mainRect.top) / 3) + (60 * 2),
                        .width = BUTTON_WIDTH_MENU,
                        .height = BUTTON_HEIGHT_MENU
                    }
                );
            }
            if (windowHandler.mainWindow != NULL && windowHandler.menuWindow != NULL && windowHandler.gameContainerWindow == NULL) {
                RECT mainRect; GetClientRect(windowHandler.mainWindow, &mainRect);
                createButton(
                    (ButtonConfig){
                        .label = L"Reset Game",
                        .parent = hwnd,
                        .ID = ID_RESET_GAME,
                        .x = mainRect.left + 40,
                        .y = mainRect.top + 40,
                        .width = BUTTON_WIDTH_GAME_RESET,
                        .height = BUTTON_HEIGHT_GAME_RESET
                    }
                );
            }
        }
        case WM_COMMAND:
        {
            switch(LOWORD(wParam)) {
                case ID_DEBUG_1:
                {
                    logDebugMessage(L"This is debug button.\n");
                    break;
                }
                case ID_BORDERLESS:
                {
                    if (windowHandler.displayMode == DISPLAY_MODE_BORDERLESS) return 0;
                    windowHandler.displayMode = DISPLAY_MODE_BORDERLESS;
                    updateDisplayMode();
                    break;
                }
                case ID_WINDOWED:
                {
                    if (windowHandler.displayMode == DISPLAY_MODE_WINDOWED) return 0;
                    windowHandler.displayMode = DISPLAY_MODE_WINDOWED;
                    updateDisplayMode();
                    break;
                }
                case ID_RESET_GAME:
                {
                    gameBoard.gameStatus = PAUSE_GAME;
                    resetGame(windowHandler.gameFieldWindow);
                    swprintf(gameBoard.score_text, 20, L"%s%d", gameBoard.score_label, gameBoard.score);
                    InvalidateRect(windowHandler.gameContainerWindow, NULL, TRUE);
                    InvalidateRect(windowHandler.gameFieldWindow, NULL, TRUE);
                }
                case ID_START_GAME:
                {
                    switchWindows(windowHandler.menuWindow, windowHandler.gameContainerWindow);
                    ShowWindow(windowHandler.gameDataDisplayWindow, SW_SHOW);
                    ShowWindow(windowHandler.gameFieldWindow, SW_SHOW);
                    ShowWindow(windowHandler.gameEnergyWindow, SW_SHOW);
                    SetFocus(windowHandler.gameFieldWindow);
                }
            }
            return 0;
        }
        case WM_SETCURSOR:
        {
            DefWindowProc(hwnd, uMsg, wParam, lParam);
            return 0;
        }
        case WM_PAINT:
        {
            if (hwnd == windowHandler.mainWindow) {
                paintMainWindow();
            }
            else if (hwnd == windowHandler.menuWindow) {
                paintMenuWindow();
            }
            else if (hwnd == windowHandler.gameContainerWindow) {
                paintGameContainerWindow();
            }
            else if (hwnd == windowHandler.gameDataDisplayWindow) {
                PaintGameDataDisplayWindow();
            }
            else if (hwnd == windowHandler.gameFieldWindow) {
                paintGameFieldWindow();
            }
            else if (hwnd == windowHandler.gameEnergyWindow) {
                paintGameEnergyWindow();
            }
            return 0;
        }
        case WM_KEYDOWN:
        {
            
            if (wParam == 'W' || wParam == 'w' || wParam == VK_UP) {
                if (gameBoard.gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_UP);
                }
            }
            else if (wParam == 'A' || wParam == 'a' || wParam == VK_LEFT) {
                if (gameBoard.gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_LEFT);
                }
            }
            else if (wParam == 'S' || wParam == 's' || wParam == VK_DOWN) {
                if (gameBoard.gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_DOWN);
                }
            }
            else if (wParam == 'D' || wParam == 'd' || wParam == VK_RIGHT) {
                if (gameBoard.gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_RIGHT);
                }
            }
            else if (wParam == VK_RETURN) {
                togglePause(windowHandler.mainWindow);
            }
            else if (wParam == VK_SHIFT) {
                if (snake.boost_depleted == FALSE) {
                    setBoost(windowHandler.mainWindow);
                    if (snake.boost_recharging == TRUE) {
                        stopBoostRecharge(windowHandler.mainWindow);
                    }
                }
            }
            return 0;
        }
        case WM_KEYUP:
        {
            if (wParam == VK_SHIFT) {
                disableBoost(windowHandler.mainWindow);
                if (snake.boost_depleted == TRUE) {
                    snake.boost_depleted = FALSE;
                }
                if (snake.boost_recharging == FALSE) {
                    startBoostRecharge(windowHandler.mainWindow);
                }
            }
        }
        case WM_TIMER:
        {
            UINT_PTR timer_val = (UINT_PTR)wParam;
            switch (timer_val) {
                case NORMAL_TICK_SPEED_TIMER_ID:
                    if (snake.boost == FALSE) {
                        if (gameBoard.gameStatus == START_GAME) {
                            if (gameBoard.update_score == TRUE) {
                                InvalidateRect(windowHandler.gameDataDisplayWindow, NULL, TRUE);
                                gameBoard.update_score = FALSE;
                            }
                            generateNextFrame(windowHandler.gameFieldWindow);
                        }
                        if (gameBoard.gameStatus == GAME_OVER) {
                            InvalidateRect(windowHandler.gameFieldWindow, NULL, TRUE);
                        }
                        InvalidateRect(windowHandler.gameEnergyWindow, NULL, TRUE);
                    }
                    break;
                case GAME_TIMER_BOOST_ID:
                    if (snake.boost == TRUE) {
                        if (gameBoard.gameStatus == START_GAME && snake.boost == TRUE) {
                            if (gameBoard.energy_level > 0) {
                                if (gameBoard.update_score == TRUE) {
                                    InvalidateRect(windowHandler.gameDataDisplayWindow, NULL, TRUE);
                                    gameBoard.update_score = FALSE;
                                }
                                generateNextFrame(windowHandler.gameFieldWindow);
                            }
                            else {
                                setBoostDepleted(windowHandler.mainWindow);
                                startBoostRecharge(windowHandler.mainWindow);
                            }
                        }
                        if (gameBoard.gameStatus == GAME_OVER) {
                            InvalidateRect(windowHandler.gameFieldWindow, NULL, TRUE);
                        }
                        updateEnergyLevel(windowHandler.mainWindow);
                        InvalidateRect(windowHandler.gameEnergyWindow, NULL, TRUE);
                    }
                    break;
                case GAME_TIMER_BOOST_RECHARGE_ID:
                    if (snake.boost_recharging == TRUE) {
                        updateEnergyLevel(windowHandler.mainWindow);
                    }
                    break;
            }
            return 0;
        }
        case WM_SIZE:
        {
            if (hwnd == windowHandler.mainWindow) {
                updateGameboardPos();
            }
            resizeAllWindows();
            resizeAllWindowBackbuffers(hwnd);
            repaintAllWindows();
            return 0;
        }
        case WM_NCCREATE:
        {
            WindowState* st = calloc(1, sizeof(WindowState));
            st->staticDirty = TRUE;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)st);
            return TRUE;  // Window creation fails if this doesn't return true.
        }
        case WM_NCDESTROY:
        {
            WindowState* st = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            free(st);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void repaintAllWindows() {
    //if (windowHandler.mainWindow) InvalidateRect(windowHandler.mainWindow, NULL, TRUE);
    if (windowHandler.menuWindow) InvalidateRect(windowHandler.menuWindow, NULL, TRUE);
    if (windowHandler.gameContainerWindow) InvalidateRect(windowHandler.gameContainerWindow, NULL, TRUE);
    if (windowHandler.gameDataDisplayWindow) InvalidateRect(windowHandler.gameDataDisplayWindow, NULL, TRUE);
    if (windowHandler.gameFieldWindow) InvalidateRect(windowHandler.gameFieldWindow, NULL, TRUE);
    if (windowHandler.gameEnergyWindow) InvalidateRect(windowHandler.gameEnergyWindow, NULL, TRUE);
}

static void resizeAllWindows() {
    if (windowHandler.menuWindow) resizeMenuWindow();
    if (windowHandler.gameContainerWindow) resizeGameContainerWindow();
    if (windowHandler.gameDataDisplayWindow) resizeGameDataDisplayWindow();
    if (windowHandler.gameFieldWindow) resizeGameFieldWindow();
    if (windowHandler.gameEnergyWindow) resizeGameEnergyWindow();
}

static void resizeAllWindowBackbuffers(HWND hwnd) {
    resizeWindowBackbuffer(hwnd);
    if (windowHandler.mainWindow && windowHandler.mainWindow != hwnd) resizeWindowBackbuffer(windowHandler.mainWindow);
    if (windowHandler.menuWindow) resizeWindowBackbuffer(windowHandler.menuWindow);
    if (windowHandler.gameContainerWindow) resizeWindowBackbuffer(windowHandler.gameContainerWindow);
    if (windowHandler.gameDataDisplayWindow) resizeWindowBackbuffer(windowHandler.gameDataDisplayWindow);
    if (windowHandler.gameFieldWindow) resizeWindowBackbuffer(windowHandler.gameFieldWindow);

    // Not necessary. Energy guage does not change size.
    if (windowHandler.gameEnergyWindow) resizeWindowBackbuffer(windowHandler.gameEnergyWindow);
}

static void resizeWindowBackbuffer(HWND hwnd) {
    WindowRECT windowRect = getSnakeWindowRect(hwnd);
    WindowState* windowST = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    windowST->width = windowRect.width;
    windowST->height = windowRect.height;
    if (windowST->width <=0 || windowST->height <= 0) return; // Window minimized.
    if (windowST->staticBmp) { // Delete the old, incorrectly sized bitmap (if one exists).
        SelectObject(windowST->staticDC, windowST->staticOldBmp);
        DeleteObject(windowST->staticBmp);
        DeleteDC(windowST->staticDC);
        windowST->staticDC      = NULL;
        windowST->staticBmp     = NULL;
        windowST->staticOldBmp  = NULL;
    }
    if (windowST->frameBmp) { // same for the frame bitmap.
        SelectObject(windowST->frameDC, windowST->frameOldBmp);
        DeleteObject(windowST->frameBmp);
        DeleteDC(windowST->frameDC);
        windowST->frameDC       = NULL;
        windowST->frameBmp      = NULL;
        windowST->frameOldBmp   = NULL;
    }
    HDC windowDC = GetDC(hwnd);

    windowST->staticDC      = CreateCompatibleDC(windowDC);
    windowST->staticBmp     = CreateCompatibleBitmap(windowDC, windowST->width, windowST->height);
    windowST->staticOldBmp  = (HBITMAP)SelectObject(windowST->staticDC, windowST->staticBmp);

    windowST->frameDC       = CreateCompatibleDC(windowDC);
    windowST->frameBmp      = CreateCompatibleBitmap(windowDC, windowST->width, windowST->height);
    windowST->frameOldBmp   = (HBITMAP)SelectObject(windowST->frameDC, windowST->frameBmp);

    ReleaseDC(hwnd, windowDC);
    windowST->staticDirty = TRUE;
}

/*
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
*/

void resizeMenuWindow() {
    WindowRECT menuWindowRect = getMenuWindowRect();
    SetWindowPos(windowHandler.menuWindow, NULL, 
        menuWindowRect.left, menuWindowRect.top, 
        menuWindowRect.width, menuWindowRect.height, 
        SWP_NOZORDER);
    
    ButtonList buttons = getButtonList(windowHandler.menuWindow);
    int buttonOffset = 0;
    int x = ((menuWindowRect.right - menuWindowRect.left) / 2) - (BUTTON_WIDTH_MENU / 2);
    int y = ((menuWindowRect.bottom - menuWindowRect.top) / 3);
    
    for (int i = 0; i < buttons.count; i++) {
        SetWindowPos(buttons.handles[i], NULL, x, y + (buttonOffset * 60), 
        BUTTON_WIDTH_MENU, BUTTON_HEIGHT_MENU, 
        SWP_NOZORDER | SWP_FRAMECHANGED);
        buttonOffset++;
    }
}

void resizeGameContainerWindow() {
    WindowRECT gameContainerWindowRect = getGameContainerWindowRect();
    SetWindowPos(windowHandler.gameContainerWindow, NULL, 
        gameContainerWindowRect.left, gameContainerWindowRect.top, 
        gameContainerWindowRect.width, gameContainerWindowRect.height, 
        SWP_NOZORDER);

    ButtonList buttons = getButtonList(windowHandler.gameContainerWindow);
    // Update position of the "reset game" button
    SetWindowPos(buttons.handles[0], NULL, gameContainerWindowRect.left + 40, gameContainerWindowRect.top + 40, 
        BUTTON_WIDTH_MENU, BUTTON_HEIGHT_MENU, 
        SWP_NOZORDER | SWP_FRAMECHANGED);
}

void resizeGameDataDisplayWindow() {
    WindowRECT gameDataDisplayWindowRect = getGameDataDisplayWindowRect();
    SetWindowPos(windowHandler.gameDataDisplayWindow, NULL, 
        gameDataDisplayWindowRect.left, gameDataDisplayWindowRect.top, 
        gameDataDisplayWindowRect.width, gameDataDisplayWindowRect.height, 
        SWP_NOZORDER);
}

void resizeGameFieldWindow() {
    WindowRECT gameFieldWindowRect = getGameFieldWindowRect();
    SetWindowPos(windowHandler.gameFieldWindow, NULL, 
        gameFieldWindowRect.left, gameFieldWindowRect.top, 
        gameFieldWindowRect.width, gameFieldWindowRect.height, 
        SWP_NOZORDER);
}

void resizeGameEnergyWindow() {
    WindowRECT gameEnergyWindowRect = getGameEnergyWindowRect();
    SetWindowPos(windowHandler.gameEnergyWindow, NULL, 
        gameEnergyWindowRect.left, gameEnergyWindowRect.top, 
        gameEnergyWindowRect.width, gameEnergyWindowRect.height, 
        SWP_NOZORDER);
}

// ((mainRect.bottom - mainRect.top) / 3)   + (buttonOffset * 60)
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) {
    wchar_t cls[32];
    GetClassNameW(hwndChild, cls, 32);
    ButtonList* list = (ButtonList*)lParam;
    if (lstrcmpiW(cls, L"Button") == 0) {
        list->handles[list->count] = hwndChild;
        list->count++;
    }
    return TRUE;
}


ButtonList getButtonList(HWND parent) {
    ButtonList list;
    list.count = 0;
    EnumChildWindows(parent, EnumChildProc, (LPARAM)&list);
    return list;
}

/**
 * @brief Initializes GDI brushes used for rendering the Snake game.
 *
 * Creates solid color brushes for all major game elements, including
 * the background, walls, game field, snake, and fruit. These brushes
 * are used by the paint functions to fill the appropriate regions.
 *
 * The colors are defined by constants such as COLOR_SNAKEGAME_BACKGROUND
 * and COLOR_SNAKEGAME_SNAKE.
 *
 * @note Brushes created here should be released later in windowCleanUp()
 *       to avoid GDI resource leaks.
 *
 * @see paintMainWindow()
 * @see paintGameWindow()
 * @see windowCleanUp()
 */
void initializeBrushes() {
    brushHandler.backgroundBrush = CreateSolidBrush(COLOR_SNAKEGAME_BACKGROUND);
    brushHandler.wallBrush = CreateSolidBrush(COLOR_SNAKEGAME_WALL);
    brushHandler.fieldBrush = CreateSolidBrush(COLOR_SNAKEGAME_GAMEFIELD);
    brushHandler.snakeBrush = CreateSolidBrush(COLOR_SNAKEGAME_SNAKE);
    brushHandler.fruitBrush = CreateSolidBrush(COLOR_SNAKEGAME_FRUIT);
}

/*----------------------------------------------------------------------------*/
/*                              Utility                                       */
/*----------------------------------------------------------------------------*/

/**
 * @brief Updates the position and dimensions of the game board within the main window.
 *
 * Retrieves the client area of the main window and passes it to updateGameboard(),
 * which recalculates the board's centered position and adjusts cell sizing based
 * on the current window dimensions.
 *
 * This function is typically called:
 * - After the main window is created (during setup).
 * - When the window is resized (on WM_SIZE).
 *
 * @see updateGameboard()
 * @see SnakeWindowProc()
 */
void updateGameboardPos() {
    RECT mainWindowRect;
    GetClientRect(windowHandler.mainWindow, &mainWindowRect);
    updateGameboard(mainWindowRect);
    GameBoardRect gameboardRect = getGameboardRect();
    MoveWindow(windowHandler.gameFieldWindow, gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height, TRUE);
}

void updateDisplayMode() {
    HWND hwndForeground = GetForegroundWindow();
    if (windowHandler.displayMode == DISPLAY_MODE_BORDERLESS) {
        SetWindowLong(windowHandler.mainWindow, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    }
    else if (windowHandler.displayMode == DISPLAY_MODE_WINDOWED) {
        SetWindowLong(windowHandler.mainWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    }
    WindowRECT mainWindowRect = getMainWindowRect();
    SetWindowPos(windowHandler.mainWindow, NULL, 
        mainWindowRect.left, mainWindowRect.top, 
        mainWindowRect.width, mainWindowRect.height,
        SWP_NOZORDER | SWP_FRAMECHANGED);
    ShowWindow(windowHandler.mainWindow, SW_RESTORE);
    SetFocus(hwndForeground);
}

void switchWindows(HWND hwnd_current, HWND hwnd_next) {
    ShowWindow(hwnd_current, SW_HIDE);
    ShowWindow(hwnd_next, SW_SHOW);
    SetFocus(hwnd_next);
}

/*----------------------------------------------------------------------------*/
/*                              Painting                                      */
/*----------------------------------------------------------------------------*/

/**
 * @brief Handles painting of the main window background and title text.
 *
 * Fills the main window with the background brush and draws the centered
 * "Snake Game" title above the game board area. This function is called
 * when the main window receives a WM_PAINT message.
 *
 * @note Only paints static UI elements outside the playable game area.
 *
 * @see paintGameWindow()
 * @see getGameboardRect()
 */
void paintMainWindow() {
    WindowState* st = (WindowState*)GetWindowLongPtr(windowHandler.mainWindow, GWLP_USERDATA);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.mainWindow, &ps);
    if (st->staticDirty == TRUE) {
        RECT winRect; GetClientRect(windowHandler.mainWindow, &winRect);
        FillRect(st->staticDC, &winRect, brushHandler.backgroundBrush);
        st->staticDirty = FALSE;
    }
    BitBlt(st->frameDC, 0, 0, st->width, st->height, st->staticDC, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);
    EndPaint(windowHandler.mainWindow, &ps);
}

void paintMenuWindow() {
    WindowState* st = (WindowState*)GetWindowLongPtr(windowHandler.menuWindow, GWLP_USERDATA);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.menuWindow, &ps);
    if (st->staticDirty == TRUE) {
        RECT winRect; GetClientRect(windowHandler.menuWindow, &winRect);
        FillRect(st->staticDC, &winRect, brushHandler.backgroundBrush);
        st->staticDirty = FALSE;
    }
    BitBlt(st->frameDC, 0, 0, st->width, st->height, st->staticDC, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);
    EndPaint(windowHandler.menuWindow, &ps);
}

void paintGameContainerWindow() {
    WindowState* st = (WindowState*)GetWindowLongPtr(windowHandler.gameContainerWindow, GWLP_USERDATA);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.gameContainerWindow, &ps);

    if (st->staticDirty == TRUE) {
        RECT winRect; GetClientRect(windowHandler.gameContainerWindow, &winRect);
        FillRect(st->staticDC, &winRect, brushHandler.backgroundBrush);
        st->staticDirty = FALSE;
    }
    BitBlt(st->frameDC, 0, 0, st->width, st->height, st->staticDC, 0, 0, SRCCOPY);
    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);
    EndPaint(windowHandler.gameContainerWindow, &ps);
}

void PaintGameDataDisplayWindow() {
    WindowState* st = (WindowState*)GetWindowLongPtr(windowHandler.gameDataDisplayWindow, GWLP_USERDATA);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.gameDataDisplayWindow, &ps);

    if (st->staticDirty == TRUE) {
        RECT winRect; GetClientRect(windowHandler.gameDataDisplayWindow, &winRect);
        FillRect(st->staticDC, &winRect, brushHandler.backgroundBrush);
        st->staticDirty = FALSE;
    }
    BitBlt(st->frameDC, 0, 0, st->width, st->height, st->staticDC, 0, 0, SRCCOPY);

    RECT dataDisplayRect; GetClientRect(windowHandler.gameDataDisplayWindow, &dataDisplayRect);
    int savedDCConfig = SaveDC(st->frameDC);
    SetBkMode(st->frameDC, TRANSPARENT);
    SetTextColor(st->frameDC, RGB(255, 255, 255));
    SelectObject(st->frameDC, gameBoard.scoreFont);
    DrawText(st->frameDC, gameBoard.score_text, -1, &dataDisplayRect, DT_LEFT | DT_SINGLELINE);
    RestoreDC(st->frameDC, savedDCConfig);

    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);
    EndPaint(windowHandler.gameDataDisplayWindow, &ps);
}

/**
 * @brief Handles painting of the game window and its visual elements.
 *
 * Draws the full game field, walls, snake, and optional debug grid.
 * Called whenever the game window receives a WM_PAINT message or
 * after a timer-driven frame update.
 *
 * @see drawGameField()
 * @see drawWalls()
 * @see drawSnake()
 * @see drawGridDebug()
 */
void paintGameFieldWindow() {
    WindowState* st = (WindowState*)GetWindowLongPtr(windowHandler.gameFieldWindow, GWLP_USERDATA);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.gameFieldWindow, &ps);
    RECT winRect; GetClientRect(windowHandler.gameFieldWindow, &winRect);

    if (st->staticDirty == TRUE) {
        drawGameField(winRect, st->staticDC);
        drawWalls(st->staticDC);
        st->staticDirty = FALSE;
    }
    BitBlt(st->frameDC, 0, 0, st->width, st->height, st->staticDC, 0, 0, SRCCOPY);

    //drawDebugGrid(gameWindowRect, hdc);
    drawSnake(st->frameDC);
    drawFruit(st->frameDC);

    if (gameBoard.gameStatus == GAME_OVER) {
        int savedDCConfig = SaveDC(st->frameDC);
        SetBkMode(st->frameDC, TRANSPARENT);
        SelectObject(st->frameDC, gameBoard.scoreFont);

        DrawOutlinedText(
            st->frameDC, 
            L"GAME OVER",
            &winRect,
            RGB(255, 0, 0),   // text color
            RGB(0, 0, 0),     // outline color
            2,                // outline thickness (1–2 is ideal)
            DT_CENTER | DT_SINGLELINE | DT_VCENTER
        );
        RestoreDC(st->frameDC, savedDCConfig);
    }

    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);

    EndPaint(windowHandler.gameFieldWindow, &ps);
}

void DrawOutlinedText(
    HDC hdc,
    LPCWSTR text,
    RECT* rc,
    COLORREF textColor,
    COLORREF outlineColor,
    int outlinePx,
    UINT format
) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, outlineColor);

    for (int dx = -outlinePx; dx <= outlinePx; dx++) {
        for (int dy = -outlinePx; dy <= outlinePx; dy++) {
            if (dx == 0 && dy == 0) continue;
            RECT rect = *rc;
            OffsetRect(&rect, dx, dy);
            DrawText(hdc, text, -1, &rect, format);
        }
    }

    SetTextColor(hdc, textColor);
    DrawText(hdc, text, -1, rc, format);
}

void paintGameEnergyWindow() {
    WindowState* st = (WindowState*)GetWindowLongPtr(windowHandler.gameEnergyWindow, GWLP_USERDATA);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.gameEnergyWindow, &ps);
    RECT containerRect; GetClientRect(windowHandler.gameContainerWindow, &containerRect);
    RECT outerRect; GetClientRect(windowHandler.gameEnergyWindow, &outerRect);
    
    RECT innerRect = outerRect;
    innerRect.left += 10; innerRect.right -= 10;
    innerRect.top += 10; innerRect.bottom -= 10;

    RECT energyRect = innerRect;
    int innerHeight = (innerRect.bottom - innerRect.top);
    double energyPercentage = (double)(gameBoard.energy_level / 100.0);
    int energyHeight = (int)(innerHeight * energyPercentage);
    energyRect.top = energyRect.bottom - energyHeight;

    COLORREF green = RGB(0, 255, 0);
    COLORREF white = RGB(255, 255, 255);
    COLORREF darkGreen = RGB(2, 87, 2);

    HBRUSH greenBrush = CreateSolidBrush(green);
    HBRUSH whiteBrush = CreateSolidBrush(white);
    HBRUSH darkGreenBrush = CreateSolidBrush(darkGreen);

    int savedDCConfig = SaveDC(st->frameDC);

    if (st->staticDirty == TRUE) {
        COLORREF black = RGB(0, 0, 0);
        HBRUSH blackBrush = CreateSolidBrush(black);
        
        RECT containerRect = outerRect;
        containerRect.left += 2; containerRect.right -= 2;
        containerRect.top += 2; containerRect.bottom -= 2;
        
        FillRect(st->staticDC, &outerRect, whiteBrush);
        FillRect(st->staticDC, &containerRect, blackBrush);
        DeleteObject(blackBrush);

        st->staticDirty = FALSE;
    }
    BitBlt(st->frameDC, 0, 0, st->width, st->height, st->staticDC, 0, 0, SRCCOPY);

    FillRect(st->frameDC, &innerRect, darkGreenBrush);
    FillRect(st->frameDC, &energyRect, greenBrush);
    drawEnergyDisplay(st->frameDC, innerRect, innerHeight);

    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);

    DeleteObject(whiteBrush);
    DeleteObject(greenBrush);
    DeleteObject(darkGreenBrush);
    RestoreDC(st->frameDC, savedDCConfig);
    EndPaint(windowHandler.gameEnergyWindow, &ps);
}

void drawEnergyDisplay(HDC hdc, RECT innerRect, int innerHeight) {
    int savedDCConfig = SaveDC(hdc);
    COLORREF white = RGB(255, 255, 255);
    COLORREF black = RGB(0, 0, 0);

    RECT energyDisplayRect;
    int innerWidth = (innerRect.right - innerRect.left);
    int energyDisplayHeight = (innerHeight / 3);
    int energyDisplayWidth = innerWidth - 4;
    int left_padding = (innerWidth - energyDisplayWidth) / 2;
    int top_padding = (innerHeight - energyDisplayHeight) / 2;
    energyDisplayRect.left = innerRect.left;
    energyDisplayRect.top = innerRect.top + top_padding;
    energyDisplayRect.right = innerRect.right;
    energyDisplayRect.bottom = energyDisplayRect.top + energyDisplayHeight;

    RECT energyValueRect;
    energyValueRect.left = energyDisplayRect.left;
    energyValueRect.right = energyDisplayRect.right;
    int energyValueHeight = energyDisplayHeight * 0.33;
    energyValueRect.top = energyDisplayRect.bottom + 10;
    energyValueRect.bottom = energyValueRect.top + energyValueHeight;

    POINT bolt[] = {
        { 70,  0 },
        { 50, 45 },
        { 70, 45 },
        { 30, 100 },
        { 45, 55 },
        { 25, 55 }
    };

    const int count = sizeof(bolt) / sizeof(bolt[0]);

    // Scale points into RECT
    POINT pts[count];
    for (int i = 0; i < count; i++) {
        pts[i].x = energyDisplayRect.left + (bolt[i].x * energyDisplayWidth) / 100;
        pts[i].y = energyDisplayRect.top  + (bolt[i].y * energyDisplayHeight) / 100;
    }

    HPEN blackPen = CreatePen(PS_SOLID, 1, black);
    HBRUSH whiteBrush = CreateSolidBrush(white);
    SelectObject(hdc, blackPen);
    SelectObject(hdc, whiteBrush);
    Polygon(hdc, pts, count);
    //Draw outline next
    for (int i = 0; i < count; i++) {
        if (i == (count - 1)) {
            MoveToEx(hdc, pts[i].x, pts[i].y, NULL);
            LineTo(hdc, pts[0].x, pts[0].y);
        }
        else {
            MoveToEx(hdc, pts[i].x, pts[i].y, NULL);
            LineTo(hdc, pts[i + 1].x, pts[i + 1].y);
        }
    }

    wchar_t energyValueString[5];
    swprintf(energyValueString, 5, L"%d", gameBoard.energy_level);

    SelectObject(hdc, gameBoard.energyFont);

    DrawOutlinedText(
        hdc, 
        energyValueString,
        &energyValueRect,
        RGB(255, 255, 255),   // text color
        RGB(0, 0, 0),     // outline color
        1,                // outline thickness (1–2 is ideal)
        DT_CENTER | DT_SINGLELINE
    );

    RestoreDC(hdc, savedDCConfig);
}

void animation_game_over(HDC hdc) {
    WindowRECT gameFieldWindowRect = getGameFieldWindowRect();
    RECT animationRect = {
        .left = gameFieldWindowRect.left,
        .top = gameFieldWindowRect.top,
        .right = gameFieldWindowRect.left + 300,
        .bottom = gameFieldWindowRect.top + 300
    };

    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hOldBrush = SelectObject(hdc, redBrush);
    Ellipse(hdc, animationRect.left, animationRect.top, animationRect.right, animationRect.bottom);

    SelectObject(hdc, hOldBrush);


    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); // thickness = 3
    hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    HPEN hOldPen = SelectObject(hdc, hPen);

    Ellipse(hdc, animationRect.left, animationRect.top, animationRect.right, animationRect.bottom);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(redBrush);

}

/*
void drawCircle(HDC hdc, RECT cell_bounds) {
    Ellipse(hdc, cell_bounds.left, cell_bounds.top, cell_bounds.right, cell_bounds.bottom);
}
*/

/**
 * @brief Draws the snake on the game board.
 *
 * Iterates through all snake nodes and renders each segment as a filled circle
 * using the snake brush. The head is drawn first, followed by the body segments.
 * Logs an error if the snake has not been initialized.
 *
 * @param hdc Handle to the device context used for drawing.
 *
 * @see getCellBoundingRect()
 * @see drawCircle()
 * @see logError()
 */
void drawSnake(HDC hdc) {
    if (snake.node == NULL) {
        logError(L"Error in function drawSnake(HDC hdc) of window.h.\n\tsnake.node == NULL\n");
    }
    SnakeNode* node = snake.node;
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHandler.snakeBrush);
    while (node != NULL) {
        RECT node_bounds = getNodeBoundingRect(node->x, node->y);
        drawCircle(hdc, node_bounds);
        node = node->nextNode;
    }
    SelectObject(hdc, oldBrush);
}

/**
 * @brief Draws the background field of the game area.
 *
 * Fills the specified rectangular region with the game field brush.
 * Typically called before drawing the snake and walls.
 *
 * @param field Rectangle defining the game field boundaries.
 * @param hdc   Handle to the device context used for drawing.
 *
 * @see paintGameWindow()
 */
void drawGameField(RECT field, HDC hdc) {
    FillRect(hdc, &field, brushHandler.fieldBrush);
}

/**
 * @brief Draws the perimeter walls around the game grid.
 *
 * Renders circular wall segments along the outermost rows and columns
 * of the grid using the wall brush. These represent the game boundaries
 * that the snake cannot cross.
 *
 * @param hdc Handle to the device context used for drawing.
 *
 * @see drawCircle()
 * @see getCellBoundingRect()
 */
void drawWalls(HDC hdc) {
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHandler.wallBrush);
    for (int i = 1; i <= GAMEGRIDCOLS; i++) {
        RECT cell_bounds = getCellBoundingRect(i, 1);
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);
        cell_bounds = getCellBoundingRect(i, GAMEGRIDCOLS);
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);

        cell_bounds = getCellBoundingRect(1, i);
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);
        cell_bounds = getCellBoundingRect(GAMEGRIDCOLS, i);
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);
    }
    SelectObject(hdc, oldBrush);
}

void drawFruit(HDC hdc) {
    // --- Fruit body (orange) ---
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHandler.fruitBrush);

    RECT fruit_bounds = getNodeBoundingRect(gameBoard.fruitLoc.x, gameBoard.fruitLoc.y);

    Ellipse(hdc, fruit_bounds.left, fruit_bounds.top, fruit_bounds.right, fruit_bounds.bottom);

    // --- Stem ---
    HPEN stemPen = CreatePen(PS_SOLID, 4, RGB(60, 120, 40));
    HPEN oldPen = (HPEN)SelectObject(hdc, stemPen);
    int stem_x = ((fruit_bounds.right - fruit_bounds.left) / 2) + fruit_bounds.left;
    int stem_y = fruit_bounds.top;
    MoveToEx(hdc, stem_x, stem_y, NULL);
    LineTo(hdc, stem_x + 10, stem_y + 10);

    /*
    // --- Leaf ---
    HBRUSH leafBrush = CreateSolidBrush(RGB(40, 160, 60));
    SelectObject(hdc, leafBrush);
    POINT leaf[4] = {
        {150, 80}, {170, 70}, {160, 60}, {150, 80}
    };
    Polygon(hdc, leaf, 4);

    */
    // --- Restore & cleanup ---
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    //DeleteObject(leafBrush);
    DeleteObject(stemPen);
}

/**
 * @brief Draws a filled circle within a specified rectangle.
 *
 * Uses the currently selected brush in the device context to render
 * a filled ellipse corresponding to the given bounding rectangle.
 *
 * @param hdc         Handle to the device context used for drawing.
 * @param cell_bounds Rectangle specifying the circle’s bounding box.
 *
 * @see drawSnake()
 * @see drawWalls()
 */
void drawCircle(HDC hdc, RECT cell_bounds) {
    Ellipse(hdc, cell_bounds.left, cell_bounds.top, cell_bounds.right, cell_bounds.bottom);
}

/**
 * @brief Draws grid lines over the game field for debugging purposes.
 *
 * Creates a temporary black pen and draws vertical and horizontal
 * grid lines to visualize the cell layout. The pen is deleted after
 * drawing to prevent GDI resource leaks.
 *
 * @param field Rectangle defining the visible game field area.
 * @param hdc   Handle to the device context used for drawing.
 *
 * @note Intended for development/debugging only. Can be disabled in release builds.
 *
 * @see getGameBoardCellWidth()
 * @see getGameBoardCellHeight()
 */
void drawDebugGrid(RECT field, HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int cell_width = getGameBoardCellWidth();
    int cell_height = getGameBoardCellHeight();
    
    for (int i = 1; i <= GAMEGRIDCOLS; i++) {
        int col = field.left + (i * cell_width);
        MoveToEx(hdc, col, field.top, NULL);
        LineTo(hdc, col, field.bottom);
    }

    for (int i = 1; i <= GAMEGRIDROWS; i++) {
        int row = field.top + (i * cell_height);
        MoveToEx(hdc, field.left, row, NULL);
        LineTo(hdc, field.right, row);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

/*----------------------------------------------------------------------------*/
/*                              Clean Up                                      */
/*----------------------------------------------------------------------------*/

/**
 * @brief Releases all window-related graphical resources.
 *
 * Cleans up and deallocates all GDI objects (brushes) created during
 * window initialization. This function should be called when the
 * application exits or when the window is destroyed (WM_DESTROY).
 *
 * @see deleteBrushes()
 * @see initializeBrushes()
 */
void windowCleanUp() {
    deleteBrushes();
}

/**
 * @brief Deletes all GDI brushes used for rendering the Snake game.
 *
 * Calls DeleteObject() on every brush handle created by initializeBrushes(),
 * including brushes for the background, walls, field, snake, and fruit.
 * Ensures proper cleanup of GDI resources to prevent memory or handle leaks.
 *
 * @note Must be called before application exit or when the main window is destroyed.
 *
 * @see initializeBrushes()
 * @see windowCleanUp()
 */
void deleteBrushes() {
    DeleteObject(brushHandler.backgroundBrush);
    DeleteObject(brushHandler.wallBrush);
    DeleteObject(brushHandler.fieldBrush);
    DeleteObject(brushHandler.snakeBrush);
    DeleteObject(brushHandler.fruitBrush);
}

#endif
