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
#define GAME_WINDOW_CLASS L"SNAKE_WIN"
#define COLOR_SNAKEGAME_BACKGROUND RGB(33, 176, 164)
#define COLOR_SNAKEGAME_GAMEFIELD RGB(255, 255, 255)
#define COLOR_SNAKEGAME_SNAKE RGB(38, 191, 51)
#define COLOR_SNAKEGAME_FRUIT RGB(235, 232, 52)
#define COLOR_SNAKEGAME_WALL RGB(0, 0, 0)
#endif




/*==============================================================================
 *                           GLOBAL  VARIABLES
 *============================================================================*/

 /**
 * @brief Handle to the main application window.
 *
 * Represents the top-level window that contains the game area and UI elements.
 * Acts as the parent of the game window and is responsible for handling
 * global events such as resizing, painting, and keyboard input.
 *
 * @see gameWindow
 * @see windowSetup()
 * @see SnakeWindowProc()
 */
HWND mainWindow;

/**
 * @brief Handle to the child window used for game rendering.
 *
 * Represents the area where all game graphics are drawn, including the grid,
 * snake, and fruit. This window is embedded inside the main application window.
 *
 * @see mainWindow
 * @see paintGameWindow()
 */
HWND gameWindow;

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
void createGameWindows(HINSTANCE hInstance);
void initializeBrushes();

/*----------------------------------------------------------------------------*/
/*                             Window Procedure                               */
/*----------------------------------------------------------------------------*/

LRESULT CALLBACK SnakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*----------------------------------------------------------------------------*/
/*                             Window Utility                                 */
/*----------------------------------------------------------------------------*/

void updateGameboardPos();

/*----------------------------------------------------------------------------*/
/*                             Window Graphics                                */
/*----------------------------------------------------------------------------*/

void paintMainWindow();
void paintUIElements();
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
    RegisterWindowClass(hInstance, MAIN_WINDOW_CLASS, SnakeWindowProc);
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
    mainWindow = CreateWindowEx(
        0,                              // Optional window styles.
        MAIN_WINDOW_CLASS,                      // Window class
        L"SNAKE",                       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
    if (mainWindow == NULL) {
        logError(L"Error in function createGameWindow() of window.h.\n\tmainWindow == NULL. Window creation failed.\n");
    }
    updateGameboardPos();
    initializeCellAndNodeData();
    RECT parentRect;
    GetClientRect(mainWindow, &parentRect);
    GameBoardRect gameboardRect = getGameboardRect();
    gameWindow = CreateWindowEx(
        0,                              // Optional window styles.
        GAME_WINDOW_CLASS,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style
        // Size and position
        gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height,
        mainWindow,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
    if (gameWindow == NULL) {
        logError(L"Error in function createGameWindow() of window.h.\n\tgameWindow == NULL. Window creation failed.\n");
    }
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
        case WM_SETCURSOR:
        {
            DefWindowProc(hwnd, uMsg, wParam, lParam);
            return 0;
        }
            
        case WM_PAINT:
        {
            if (hwnd == mainWindow) {
                paintMainWindow();
            }
            else {
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
                generateNextFrame();
                RECT gameField; GetClientRect(gameWindow, &gameField);
                gameField.left = gameBoard.cell_width;
                gameField.right = gameField.right - gameBoard.cell_width;
                gameField.top = gameBoard.cell_height;
                gameField.bottom = gameField.bottom - gameBoard.cell_height;
                InvalidateRect(gameWindow, &gameField, TRUE);
            }
            return 0;
        }
        case WM_SIZE:
        {
            updateGameboardPos();
            InvalidateRect(mainWindow, NULL, TRUE);
            InvalidateRect(gameWindow, NULL, TRUE);
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
    GetClientRect(mainWindow, &mainWindowRect);
    updateGameboard(mainWindowRect);
    GameBoardRect gameboardRect = getGameboardRect();
    MoveWindow(gameWindow, gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height, TRUE);
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
    HDC hdc = BeginPaint(mainWindow, &ps);
    FillRect(hdc, &ps.rcPaint, backgroundBrush);

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


    EndPaint(mainWindow, &ps);
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
    HDC hdc = BeginPaint(gameWindow, &ps);
    RECT gameWindowRect;
    GetClientRect(gameWindow, &gameWindowRect);
    drawGameField(gameWindowRect, hdc);
    drawWalls(hdc);
    //drawDebugGrid(gameWindowRect, hdc);
    drawSnake(hdc);
    drawFruit(hdc);
    EndPaint(gameWindow, &ps);
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
