#ifndef WINDOW_H
#define WINDOW_H

// Ensure Windows APIs use Unicode versions
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "game.h"

// ******************** Macros ********************

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

/* ************************************************************ */

// ******************** Global Variables ********************

HWND mainWindow;
HWND gameWindow;
HBRUSH backgroundBrush;
HBRUSH wallBrush;
HBRUSH fieldBrush;
HBRUSH snakeBrush;
HBRUSH fruitBrush;

/* ************************************************************ */

// ******************** Function Prototypes ********************

/*   --- Setup ---   */
void windowSetup(HINSTANCE hInstance); // - Wrapper
void RegisterWindowClass (HINSTANCE hInstance, const wchar_t * className, WNDPROC windowProc);
void createGameWindows(HINSTANCE hInstance);
LRESULT CALLBACK SnakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void initializeBrushes();
/*   -------------   */

/*   --- Utility ---   */
void updateGameboardPos();
/*   -------------   */

/*   --- Painting ---   */
void paintMainWindow();
void paintUIElements();
void paintGameWindow();
void drawGridDebug(RECT field, HDC hdc);
void drawGameField(RECT field, HDC hdc);
void drawSnake(HDC hdc);
void drawWalls(HDC hdc);
void drawCircle(HDC hdc, RECT cell_bounds);
/*   -------------   */

/*   --- Clean Up ---   */
void windowCleanUp(); //- Wrapper
void deleteBrushes();
/*   -------------   */

/* ************************************************************ */

// ******************** Function Implementations ********************

/*   --- Setup ---   */
void windowSetup(HINSTANCE hInstance) {
    RegisterWindowClass(hInstance, MAIN_WINDOW_CLASS, SnakeWindowProc);
    RegisterWindowClass(hInstance, GAME_WINDOW_CLASS, SnakeWindowProc);
    createGameWindows(hInstance);
    initializeBrushes();
}

void RegisterWindowClass (HINSTANCE hInstance, const wchar_t * className, WNDPROC windowProc) {
    WNDCLASS wc = { };

    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);
}

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
            GameBoardRect gameboardRect = getGameboardRect();
            MoveWindow(gameWindow, gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height, TRUE);
            InvalidateRect(mainWindow, NULL, TRUE);
            InvalidateRect(gameWindow, NULL, TRUE);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void initializeBrushes() {
    backgroundBrush = CreateSolidBrush(COLOR_SNAKEGAME_BACKGROUND);
    wallBrush = CreateSolidBrush(COLOR_SNAKEGAME_WALL);
    fieldBrush = CreateSolidBrush(COLOR_SNAKEGAME_GAMEFIELD);
    snakeBrush = CreateSolidBrush(COLOR_SNAKEGAME_SNAKE);
    fruitBrush = CreateSolidBrush(COLOR_SNAKEGAME_FRUIT);
}
/*   -------------   */

/*   --- Utility ---   */
void updateGameboardPos() {
    RECT mainWindowRect;
    GetClientRect(mainWindow, &mainWindowRect);
    updateGameboard(mainWindowRect);
}
/*   -------------   */

/*   --- Painting ---   */
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

void paintGameWindow() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(gameWindow, &ps);
    RECT gameWindowRect;
    GetClientRect(gameWindow, &gameWindowRect);
    drawGameField(gameWindowRect, hdc);
    drawWalls(hdc);
    drawGridDebug(gameWindowRect, hdc);
    drawSnake(hdc);
    EndPaint(gameWindow, &ps);
}

void paintUIElements() {
    return;
}

void drawSnake(HDC hdc) {
    if (snake.node == NULL) {
        logError(L"Error in function drawSnake(HDC hdc) of window.h.\n\tsnake.node == NULL\n");
    }
    SnakeNode* node = snake.node;
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, snakeBrush);
    while (node != NULL) {
        RECT cell_bounds = getCellBoundingRect(node->x, node->y);
        drawCircle(hdc, cell_bounds);
        node = node->nextNode;
    }
    SelectObject(hdc, oldBrush);
}

void drawGameField(RECT field, HDC hdc) {
    FillRect(hdc, &field, fieldBrush);
}

void drawWalls(HDC hdc) {
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, wallBrush);
    for (int i = 1; i <= GAMEGRIDCOLS; i++) {
        RECT cell_bounds = getCellBoundingRect(i, 1);
        drawCircle(hdc, cell_bounds);
        cell_bounds = getCellBoundingRect(i, GAMEGRIDCOLS);
        drawCircle(hdc, cell_bounds);

        cell_bounds = getCellBoundingRect(1, i);
        drawCircle(hdc, cell_bounds);
        cell_bounds = getCellBoundingRect(GAMEGRIDCOLS, i);
        drawCircle(hdc, cell_bounds);
    }
    SelectObject(hdc, oldBrush);
}

void drawCircle(HDC hdc, RECT cell_bounds) {
    Ellipse(hdc, cell_bounds.left, cell_bounds.top, cell_bounds.right, cell_bounds.bottom);
}

void drawGridDebug(RECT field, HDC hdc) {
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
/*   -------------   */

/*   --- Clean Up ---   */
void windowCleanUp() {
    deleteBrushes();
}
void deleteBrushes() {
    DeleteObject(backgroundBrush);
    DeleteObject(wallBrush);
    DeleteObject(fieldBrush);
    DeleteObject(snakeBrush);
    DeleteObject(fruitBrush);
}
/*   -------------   */

/* ************************************************************ */



#endif