#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include "game.h"

// Ensure Windows APIs use Unicode versions
#ifndef UNICODE
#define UNICODE
#endif



// ******************** Typedefs ********************

typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

/* ************************************************************ */





// ******************** Macros ********************

/*
#define DEBUG

#ifdef DEBUG
    logDebugMessage(L"Running drawFrame()...\n\n");
#endif
*/

/* ************************************************************ */







// ******************** Structs ********************

/* ************************************************************ */





// ******************** Global Variables ********************

HWND mainWindow;
HWND gameWindow;

/* ************************************************************ */




// ******************** Function Prototypes ********************

// Main Window Setup Prototypes
HWND CreateMainWindow(const wchar_t * className, HINSTANCE hInstance);
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void paintMainWindow(HWND hwnd);
void RegisterWindowClass (HINSTANCE hInstance, const wchar_t * className, WNDPROC windowProc);

// Game Window Setup Prototypes
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateGameBoardWindow(HWND hwnd, const wchar_t * className, HINSTANCE hInstance);


void paintGameWindow(HWND gameWindow);
void drawGridDebug(RECT field, HDC hdc);
void drawGameField(RECT field, HDC hdc);
void drawSnake(HDC hdc);
void drawWalls(HDC hdc);
void drawCircle(HDC hdc, RECT cell_bounds);

/* ************************************************************ */









// ******************** Main Window Setup ********************
HWND CreateMainWindow(const wchar_t * className, HINSTANCE hInstance) {
    return CreateWindowEx(
        0,                              // Optional window styles.
        className,                      // Window class
        L"SNAKE",                       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

void RegisterWindowClass (HINSTANCE hInstance, const wchar_t * className, WNDPROC windowProc) {
    WNDCLASS wc = { };

    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    
    case WM_SETCURSOR:
        DefWindowProc(hwnd, uMsg, wParam, lParam);
        return 0;
    case WM_PAINT:
    {
        paintMainWindow(hwnd);
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
            togglePause();
        }
        return 0;
    }
    case WM_TIMER:
    {
        if (gameStatus == START_GAME) {
            logDebugMessage(L"Game Started!\n\n");
            updateState(mainWindow, gameWindow);
        }
        else {
            logDebugMessage(L"Game paused.\n\n");
        }
        return 0;
    }
    case WM_SIZE:
    {
        updateGameboardRect(mainWindow);
        GameBoardRect gameboardRect = getGameBoardRect();
        MoveWindow(gameWindow, gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height, TRUE);
        InvalidateRect(mainWindow, NULL, TRUE);
        InvalidateRect(gameWindow, NULL, TRUE);
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void paintMainWindow(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &ps.rcPaint, backgroundBrush);

    const wchar_t *title = L"Snake Game";

    GameBoardRect gameBoard = getGameBoardRect();

    RECT textRect = {
        gameBoard.left,
        gameBoard.top - 40,  // 40 px above square
        gameBoard.right,
        gameBoard.top
    };

    SetBkMode(hdc, TRANSPARENT); // don’t paint background behind text
    SetTextColor(hdc, RGB(0, 0, 0)); // black text

    DrawText(hdc, title, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


    EndPaint(hwnd, &ps);
}





/* ************************************************************ */



// ******************** Game Window Setup ********************

HWND CreateGameBoardWindow(HWND hwnd, const wchar_t * className, HINSTANCE hInstance) {
    RECT parentRect;
    GetClientRect(hwnd, &parentRect);
    GameBoardRect gameboardRect = getGameBoardRect();
    HWND gameBoardWindow = CreateWindowEx(
        0,                              // Optional window styles.
        className,                      // Window class
        NULL,                       // Window text
        WS_CHILD | WS_VISIBLE,            // Window style

        // Size and position
        gameboardRect.left, gameboardRect.top, gameboardRect.width, gameboardRect.height,

        hwnd,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
    gameBoard.window = gameBoardWindow;
    return gameBoardWindow;
}

LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL firstPaint = TRUE;
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SETCURSOR:
            DefWindowProc(hwnd, uMsg, wParam, lParam);
            return 0;
        case WM_PAINT:
        {
            paintGameWindow(gameWindow);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void paintGameWindow(HWND gameWindow) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(gameWindow, &ps);
    RECT gameWindowRect;
    GetClientRect(gameWindow, &gameWindowRect);
    drawGameField(gameWindowRect, hdc);
    drawWalls(hdc);
    //drawGridDebug(gameWindowRect, hdc);
    drawSnake(hdc);
    EndPaint(gameWindow, &ps);
}

void drawSnake(HDC hdc) {
    if (snake.node == NULL) {
        logError(L"Error in function drawSnake(HDC hdc) of window.h.\n\tsnake.node == NULL\n");
    }
    SnakeNode* node = snake.node;
    HBRUSH oldBrush = SelectObject(hdc, snakeBrush);
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
    HBRUSH oldBrush = SelectObject(hdc, wallBrush);
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
    HPEN hOldPen = SelectObject(hdc, hPen);

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

/* ************************************************************ */






#endif