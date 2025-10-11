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
#define COLOR_SNAKEGAME_FRUIT RGB(255, 130, 0)
#define COLOR_SNAKEGAME_WALL RGB(0, 0, 0)

#define DISPLAY_MODE_WINDOWED       9000
#define DISPLAY_MODE_BORDERLESS     9001


#define ID_DEBUG_1      10001
#define ID_WINDOWED     10002
#define ID_BORDERLESS   10003
#define ID_START_GAME   10004
#endif


typedef struct ButtonConfig {
    wchar_t label[30];
    HWND parent;
    int ID;
    int x;
    int y;
    int width;
    int height;
} ButtonConfig;

typedef struct WindowConfig {
    int x;
    int y;
    int width;
    int height;
} WindowConfig;

typedef struct WindowHandler {
    HWND mainWindow;
    HWND menuWindow;
    HWND gameWindow;
    WindowConfig windowedConfig;
    WindowConfig borderlessConfig;
    WindowConfig minWindowedConfig;
    int displayMode;
} WindowHandler;


/*==============================================================================
 *                           GLOBAL  VARIABLES
 *============================================================================*/

WindowHandler windowHandler;
/**
 * @brief GDI brush used to paint the main window background.
 *
 * Fills the background of the main window behind the game area.
 * Created in initializeBrushes() and deleted in deleteBrushes().
 *
 * @see initializeBrushes()
 * @see deleteBrushes()
 */
HBRUSH backgroundBrush;

/**
 * @brief GDI brush used to paint the wall segments of the game board.
 *
 * Used by drawWalls() to render the border cells that enclose the playing field.
 * Created in initializeBrushes() and deleted in deleteBrushes().
 *
 * @see drawWalls()
 * @see initializeBrushes()
 * @see deleteBrushes()
 */
HBRUSH wallBrush;

/**
 * @brief GDI brush used to paint the game field background.
 *
 * Used by drawGameField() to fill the main playable area of the grid.
 * Created in initializeBrushes() and deleted in deleteBrushes().
 *
 * @see drawGameField()
 * @see initializeBrushes()
 * @see deleteBrushes()
 */
HBRUSH fieldBrush;

/**
 * @brief GDI brush used to paint the snake.
 *
 * Applied in drawSnake() to render each snake segment as a filled circle.
 * Created in initializeBrushes() and deleted in deleteBrushes().
 *
 * @see drawSnake()
 * @see initializeBrushes()
 * @see deleteBrushes()
 */
HBRUSH snakeBrush;

/**
 * @brief GDI brush used to paint fruit objects on the grid.
 *
 * Used in drawGameWindow() and drawSnake() to render fruit positions on
 * the game board. Created in initializeBrushes() and deleted in deleteBrushes().
 *
 * @see drawGameWindow()
 * @see initializeBrushes()
 * @see deleteBrushes()
 */
HBRUSH fruitBrush;


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
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t *className, WNDPROC windowProc);
void setWindowConfigs();
void createGameWindows(HINSTANCE hInstance);
void buildMainWindow(HINSTANCE hInstance);
void buildMenuWindow(HINSTANCE hInstance);
void buildGameWindow(HINSTANCE hInstance);
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
void updateWindowConfigs();
void changeDisplayMode();
void setBorderlessMode();
void setWindowedMode();

/*----------------------------------------------------------------------------*/
/*                             Window Graphics                                */
/*----------------------------------------------------------------------------*/

void paintMainWindow();
void paintUIElements();
void paintMenuWindow();
void paintGameWindow();
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

void windowCleanUp(void);
void deleteBrushes(void);

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
 * @see createGameWindows()
 * @see initializeBrushes()
 */
void windowSetup(HINSTANCE hInstance) {
    setWindowConfigs();
    RegisterWindowClass(hInstance, MAIN_WINDOW_CLASS, SnakeWindowProc);
    RegisterWindowClass(hInstance, MENU_WINDOW_CLASS, SnakeWindowProc);
    RegisterWindowClass(hInstance, GAME_WINDOW_CLASS, SnakeWindowProc);
    createGameWindows(hInstance);
    initializeBrushes();
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
 * @see createGameWindows()
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
    windowHandler.mainWindow = NULL; windowHandler.menuWindow = NULL; windowHandler.gameWindow = NULL;
    windowHandler.displayMode = DISPLAY_MODE_WINDOWED;
    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    windowHandler.borderlessConfig.width = screenWidth;
    windowHandler.borderlessConfig.height = screenHeight;
    windowHandler.borderlessConfig.x = 0;
    windowHandler.borderlessConfig.y = 0;

    windowHandler.minWindowedConfig.width = GAMEBOARDWIDTH + 300;
    windowHandler.minWindowedConfig.height = GAMEBOARDHEIGHT + 600;
    windowHandler.minWindowedConfig.x = (screenWidth / 2) - (windowHandler.minWindowedConfig.width / 2);
    windowHandler.minWindowedConfig.y = (screenHeight / 2) - (windowHandler.minWindowedConfig.height / 2);

    windowHandler.windowedConfig.width = windowHandler.minWindowedConfig.width;
    windowHandler.windowedConfig.height = windowHandler.minWindowedConfig.height;
    windowHandler.windowedConfig.x = windowHandler.minWindowedConfig.x;
    windowHandler.windowedConfig.y = windowHandler.minWindowedConfig.y;
    logDebugMessage(L"Logging initial window config:\n");
    debugDropWindowedConfig();
    logDebugMessage(L"\n");
}

void updateWindowConfigs() {
    HMONITOR hMonitor = MonitorFromWindow(windowHandler.mainWindow, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mInfo;
    int screenWidth, screenHeight;
    if (GetMonitorInfo(hMonitor, &mInfo)) {
        screenWidth = mInfo.rcMonitor.right - mInfo.rcMonitor.left;
        screenHeight = mInfo.rcMonitor.bottom - mInfo.rcMonitor.top;
    }
    else {
        screenWidth  = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);
    }
    if (windowHandler.displayMode == DISPLAY_MODE_WINDOWED) {
        logDebugMessage(L"Here 1\n\n");
        windowHandler.borderlessConfig.width = screenWidth;
        windowHandler.borderlessConfig.height = screenHeight;
        windowHandler.borderlessConfig.x = 0;
        windowHandler.borderlessConfig.y = 0;
    }
    else if (windowHandler.displayMode == DISPLAY_MODE_BORDERLESS) {
        logDebugMessage(L"Here 2\n\n");
        RECT windowedRect; GetWindowRect(windowHandler.mainWindow, &windowedRect);
        windowHandler.windowedConfig.width = windowedRect.right - windowedRect.left;
        windowHandler.windowedConfig.height = windowedRect.bottom - windowedRect.top;
        windowHandler.windowedConfig.x = windowedRect.left;
        windowHandler.windowedConfig.y = windowedRect.top;
    }
}

void changeDisplayMode() {
    updateWindowConfigs();
    if (windowHandler.displayMode == DISPLAY_MODE_BORDERLESS) {
        setBorderlessMode();
    }
    else if (windowHandler.displayMode == DISPLAY_MODE_WINDOWED) {
        setWindowedMode();
    }
}

void setBorderlessMode() {
    HWND hwndForeground = GetForegroundWindow();
    SetWindowLong(windowHandler.mainWindow, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(windowHandler.mainWindow, HWND_TOP,
        windowHandler.borderlessConfig.x, windowHandler.borderlessConfig.y,
        windowHandler.borderlessConfig.width,
        windowHandler.borderlessConfig.height,
        SWP_NOZORDER | SWP_FRAMECHANGED);
    SetFocus(hwndForeground);
}

void setWindowedMode() {
    HWND hwndForeground = GetForegroundWindow();
    SetWindowLong(windowHandler.mainWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    SetWindowPos(windowHandler.mainWindow, NULL, 
        windowHandler.windowedConfig.x, windowHandler.windowedConfig.y, 
        windowHandler.windowedConfig.width, windowHandler.windowedConfig.height,
        SWP_NOZORDER | SWP_FRAMECHANGED);
    ShowWindow(windowHandler.mainWindow, SW_RESTORE);
    SetFocus(hwndForeground);
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
void createGameWindows(HINSTANCE hInstance) {
    buildMainWindow(hInstance);
    if (windowHandler.mainWindow == NULL) {
        logError(L"Error in function createGameWindow() of window.h.\n\tmainWindow == NULL. Window creation failed.\n");
    }
    updateGameboardPos();
    initializeCellAndNodeData();
    ///*
    buildMenuWindow(hInstance);
    if (windowHandler.mainWindow == NULL) {
        logError(L"Error in function createGameWindow() of window.h.\n\tmenuWindow == NULL. Window creation failed.\n");
    }
    //*/
    buildGameWindow(hInstance);
    if (windowHandler.gameWindow == NULL) {
        logError(L"Error in function createGameWindow() of window.h.\n\tgameWindow == NULL. Window creation failed.\n");
    }
    ShowWindow(windowHandler.mainWindow, SW_SHOW);
    ShowWindow(windowHandler.menuWindow, SW_SHOW);
    ShowWindow(windowHandler.gameWindow, SW_HIDE);
}

void buildMainWindow(HINSTANCE hInstance) {
    windowHandler.mainWindow = CreateWindowEx(
        0,                              // Optional window styles.
        MAIN_WINDOW_CLASS,                      // Window class
        L"SNAKE",                       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        windowHandler.windowedConfig.x, windowHandler.windowedConfig.y, 
        windowHandler.windowedConfig.width, windowHandler.windowedConfig.height,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

void buildMenuWindow(HINSTANCE hInstance) {
    RECT mainRect; GetClientRect(windowHandler.mainWindow, &mainRect);
    windowHandler.menuWindow = CreateWindowEx(
        0,                              // Optional window styles.
        MENU_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        mainRect.left, mainRect.top, 
        mainRect.right - mainRect.left, mainRect.bottom - mainRect.top,
        windowHandler.mainWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

void buildGameWindow(HINSTANCE hInstance) {
    RECT parentRect;
    GetClientRect(windowHandler.mainWindow, &parentRect);
    GameBoardRect gameboardRect = getGameboardRect();
    windowHandler.gameWindow = CreateWindowEx(
        0,                              // Optional window styles.
        GAME_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height,
        windowHandler.mainWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

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
    swprintf(msg, 700, L"%swindowHandler.windowedConfig.x = %d. windowHandler.windowedConfig.y = %d.\n", msg, windowHandler.windowedConfig.x, windowHandler.windowedConfig.y);
    swprintf(msg, 700, L"%swindowHandler.windowedConfig.width = %d. windowHandler.windowedConfig.height = %d.\n\n", msg, windowHandler.windowedConfig.width, windowHandler.windowedConfig.height);
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
                        .width = 100,
                        .height = 30
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
                        .width = 100,
                        .height = 30
                    }
                );
                createButton(
                    (ButtonConfig){
                        .label = L"Windowed Mode",
                        .parent = hwnd,
                        .ID = ID_WINDOWED,
                        .x = ((mainRect.right - mainRect.left) / 2) - 50,
                        .y = ((mainRect.bottom - mainRect.top) / 3) + 60,
                        .width = 100,
                        .height = 30
                    }
                );
                createButton(
                    (ButtonConfig){
                        .label = L"Borderless Mode",
                        .parent = hwnd,
                        .ID = ID_BORDERLESS,
                        .x = ((mainRect.right - mainRect.left) / 2) - 50,
                        .y = ((mainRect.bottom - mainRect.top) / 3) + (60 * 2),
                        .width = 100,
                        .height = 30
                    }
                );
            }
            if (windowHandler.mainWindow != NULL && windowHandler.menuWindow != NULL && windowHandler.gameWindow == NULL) {
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
                    logDebugMessage(L"Switching to Borderless mode...\n");
                    debugDropWindowedConfig();
                    windowHandler.displayMode = DISPLAY_MODE_BORDERLESS;
                    changeDisplayMode();
                    break;
                }
                case ID_WINDOWED:
                {
                    if (windowHandler.displayMode == DISPLAY_MODE_WINDOWED) return 0;
                    logDebugMessage(L"Switching to Windowed mode...\n");
                    debugDropWindowedConfig();
                    windowHandler.displayMode = DISPLAY_MODE_WINDOWED;
                    changeDisplayMode();
                    break;
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
            else if (hwnd == windowHandler.gameWindow) {
                /*
                RECT windowRect; GetClientRect(hwnd, &windowRect);
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, fruitBrush);
                */
                paintGameWindow();
            }
            return 0;
        }
        case WM_KEYDOWN:
        {
            
            if (wParam == 'W' || wParam == 'w' || wParam == VK_UP) {
                if (gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_UP);
                }
            }
            else if (wParam == 'A' || wParam == 'a' || wParam == VK_LEFT) {
                if (gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_LEFT);
                }
            }
            else if (wParam == 'S' || wParam == 's' || wParam == VK_DOWN) {
                if (gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_DOWN);
                }
            }
            else if (wParam == 'D' || wParam == 'd' || wParam == VK_RIGHT) {
                if (gameStatus != PAUSE_GAME) {
                    changeSnakeDirection(DIRECTION_RIGHT);
                }
            }
            else if (wParam == VK_RETURN) {
                /*
                wchar_t errMsg[256];
                if (hwnd == mainWindow) {
                    swprintf(errMsg, 256, L"mainWindow window received WM_KEYDOWN signal.\n");
                }
                if (hwnd == gameWindow) {
                    swprintf(errMsg, 256, L"gameWindow window received WM_KEYDOWN signal.\n");
                }
                logDebugMessage(errMsg);
                */
                togglePause();
            }
            return 0;
        }
        case WM_TIMER:
        {
            if (gameStatus == START_GAME) {
                if (generateNextFrame() == 1) {
                    InvalidateRect(windowHandler.gameWindow, NULL, TRUE);
                }
                else {
                    RECT gameField; GetClientRect(windowHandler.gameWindow, &gameField);
                    gameField.left = gameBoard.cell_width;
                    gameField.right = gameField.right - gameBoard.cell_width;
                    gameField.top = gameBoard.cell_height;
                    gameField.bottom = gameField.bottom - gameBoard.cell_height;
                    InvalidateRect(windowHandler.gameWindow, &gameField, TRUE);
                }
            }
            return 0;
        }
        case WM_SIZE:
        {
            updateGameboardPos();
            InvalidateRect(windowHandler.mainWindow, NULL, TRUE);
            InvalidateRect(windowHandler.gameWindow, NULL, TRUE);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
    backgroundBrush = CreateSolidBrush(COLOR_SNAKEGAME_BACKGROUND);
    wallBrush = CreateSolidBrush(COLOR_SNAKEGAME_WALL);
    fieldBrush = CreateSolidBrush(COLOR_SNAKEGAME_GAMEFIELD);
    snakeBrush = CreateSolidBrush(COLOR_SNAKEGAME_SNAKE);
    fruitBrush = CreateSolidBrush(COLOR_SNAKEGAME_FRUIT);
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
    MoveWindow(windowHandler.gameWindow, gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height, TRUE);
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
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.mainWindow, &ps);
    FillRect(hdc, &ps.rcPaint, backgroundBrush);

    /*
    const wchar_t *title = L"Snake Game";

    GameBoardRect gameBoard = getGameboardRect();

    RECT textRect = {
        gameBoard.left,
        gameBoard.top - 40,  // 40 px above square
        gameBoard.right,
        gameBoard.top
    };

    SetBkMode(hdc, TRANSPARENT); // don’t paint background behind text
    SetTextColor(hdc, RGB(0, 0, 0)); // black text

    DrawText(hdc, title, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    */


    EndPaint(windowHandler.mainWindow, &ps);
}

void paintMenuWindow() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.menuWindow, &ps);
    FillRect(hdc, &ps.rcPaint, fruitBrush);
    EndPaint(windowHandler.menuWindow, &ps);
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
void paintGameWindow() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(windowHandler.gameWindow, &ps);
    RECT gameWindowRect;
    GetClientRect(windowHandler.gameWindow, &gameWindowRect);
    drawGameField(gameWindowRect, hdc);
    //drawDebugGrid(gameWindowRect, hdc);
    drawSnake(hdc);
    drawWalls(hdc);
    drawFruit(hdc);
    EndPaint(windowHandler.gameWindow, &ps);
}

/**
 * @brief Placeholder for drawing future UI elements.
 *
 * Currently unused. Reserved for potential future additions such as
 * score display, pause indicators, or other overlay UI elements.
 */
void paintUIElements() {
    return;
}

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
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, snakeBrush);
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
    FillRect(hdc, &field, fieldBrush);
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
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, wallBrush);
    for (int i = 1; i <= GAMEGRIDCOLS; i++) {
        RECT cell_bounds = getCellBoundingRect(i, 1);
        FillRect(hdc, &cell_bounds, wallBrush);
        cell_bounds = getCellBoundingRect(i, GAMEGRIDCOLS);
        FillRect(hdc, &cell_bounds, wallBrush);

        cell_bounds = getCellBoundingRect(1, i);
        FillRect(hdc, &cell_bounds, wallBrush);
        cell_bounds = getCellBoundingRect(GAMEGRIDCOLS, i);
        FillRect(hdc, &cell_bounds, wallBrush);
    }
    SelectObject(hdc, oldBrush);
}

void drawFruit(HDC hdc) {
    // --- Fruit body (orange) ---
    HBRUSH orangeFruitBrush = CreateSolidBrush(RGB(255, 140, 0));  // orange color
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, orangeFruitBrush);

    RECT fruit_bounds = getNodeBoundingRect(gameFruit.x, gameFruit.y);

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
    DeleteObject(orangeFruitBrush);
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
    DeleteObject(backgroundBrush);
    DeleteObject(wallBrush);
    DeleteObject(fieldBrush);
    DeleteObject(snakeBrush);
    DeleteObject(fruitBrush);
}

#endif
