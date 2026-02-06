/*
 * snakeWin32.c
 * --------
 * Win32 windowing + GDI rendering for Snake.
 * Owns window creation, message handling, resizing, and drawing.
 * Does NOT implement game rules or update game state.
 */


#include <stdio.h>
#include "snakeWin32.h"
#include "platform.h"

// ==================== Globals ====================

WindowHandler windowHandler;
BrushHandler brushHandler;
AnimationHandler animationHandler;
int counter = 0;
static onWindowCloseCallbackFn global_onClose = NULL;

static void repaintAllWindows();
static void resizeAllWindows();
static void resizeAllWindowBackbuffers(HWND hwnd);
static void resizeWindowBackbuffer(HWND hwnd);

/*==============================================================================
 *                           FUNCTION  IMPLEMENTATIONS
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*                              Setup                                         */
/*----------------------------------------------------------------------------*/


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

void loadSingleFont(LPCWSTR name, DWORD  fl, PVOID  res) {
    int fontsLoaded = AddFontResourceEx(name, fl, res);
    if (fontsLoaded == 0) platform_LogErrorMessage("Error loading font.\n\tFont loading failed.\n");
}

void loadFonts() {
    loadSingleFont(L"assets\\font\\Jersey_25\\Jersey25-Regular.ttf", FR_PRIVATE, NULL);
    loadSingleFont(L"assets\\font\\DM_Serif_Text\\DMSerifText-Regular.ttf", FR_PRIVATE, NULL);
    loadSingleFont(L"assets\\font\\DM_Serif_Text\\DMSerifText-Italic.ttf", FR_PRIVATE, NULL);
    HFONT scoreFont = CreateFont(
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
    platform_LoadScoreFont(scoreFont);
    if (platform_IsScoreFontNull()) {
        platform_LogErrorMessage("Error creating font (gameBoard.scoreFont) in windowSetup().\n\tFont creation failed.\n");
    }

    HFONT energyFont = CreateFont(
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
    platform_LoadEnergyFont(energyFont);
    if (platform_IsEnergyFontNull()) {
        platform_LogErrorMessage("Error creating font (gameBoard.energyFont) in windowSetup().\n\tFont creation failed.\n");
    }
}


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

    windowHandler.minWindowedConfig.width = platform_GetGameBoardWidth() + 300;
    windowHandler.minWindowedConfig.height = platform_GetGameBoardHeight() + 600;
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


void buildWindows(HINSTANCE hInstance) {

    buildMainWindow(hInstance);
    if (windowHandler.mainWindow == NULL) {
        platform_LogErrorMessage("Error in function buildWindows() of snakeWin32.h.\n\tmainWindow == NULL. Window creation failed.\n");
    }
    platform_UpdateGameboardPosition();
    platform_InitializeCellAndNodeData();
    buildMenuWindow(hInstance);
    if (windowHandler.menuWindow == NULL) {
        platform_LogErrorMessage("Error in function buildWindows() of snakeWin32.h.\n\tmenuWindow == NULL. Window creation failed.\n");
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
        platform_LogErrorMessage("Error in function buildGameWindows() of snakeWin32.h.\n\tgameContainerWindow == NULL. Window creation failed.\n");
    }
    buildGameDataDisplayWindow(hInstance);
    if (windowHandler.gameDataDisplayWindow == NULL) {
        platform_LogErrorMessage("Error in function buildGameWindows() of snakeWin32.h.\n\tgameDataDisplayWindow == NULL. Window creation failed.\n");
    }
    buildGameFieldWindow(hInstance);
    if (windowHandler.gameFieldWindow == NULL) {
        platform_LogErrorMessage("Error in function buildGameWindows() of snakeWin32.h.\n\tgameFieldWindow == NULL. Window creation failed.\n");
    }
    buildGameEnergyWindow(hInstance);
    if (windowHandler.gameEnergyWindow == NULL) {
        platform_LogErrorMessage("Error in function buildGameWindows() of snakeWin32.h.\n\tgameEnergyWindow == NULL. Window creation failed.\n");
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
    RECT gameboardRect = platform_GetGameBoardRect();
    int width = gameboardRect.right - gameboardRect.left;
    int height = gameboardRect.bottom - gameboardRect.top;

    WindowRECT gameFieldWindowRect = {
        .left = gameboardRect.left,
        .top = gameboardRect.top,
        .right = gameboardRect.left + width,
        .bottom = gameboardRect.top + height,
        .width = width,
        .height = height
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


LRESULT CALLBACK SnakeWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            windowCleanUp();
            global_onClose();
            PostQuitMessage(0);
            return 0;
        }
        case WM_CREATE:
        {
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
                    platform_SetGameStatus_pauseGame();
                    platform_ResetGame();
                    platform_UpdateScoreText();
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
                if (!platform_IsPauseGame()) {
                    platform_SetDirection(DIRECTION_UP);
                }
            }
            else if (wParam == 'A' || wParam == 'a' || wParam == VK_LEFT) {
                if (!platform_IsPauseGame()) {
                    platform_SetDirection(DIRECTION_LEFT);
                }
            }
            else if (wParam == 'S' || wParam == 's' || wParam == VK_DOWN) {
                if (!platform_IsPauseGame()) {
                    platform_SetDirection(DIRECTION_DOWN);
                }
            }
            else if (wParam == 'D' || wParam == 'd' || wParam == VK_RIGHT) {
                if (!platform_IsPauseGame()) {
                    platform_SetDirection(DIRECTION_RIGHT);
                }
            }
            else if (wParam == VK_RETURN) {
                platform_TogglePause();
            }
            else if (wParam == VK_SHIFT) {
                if (!platform_IsBoostDepleted()) {
                    platform_EnableBoost();
                    if (platform_IsBoostRecharging()) {
                        platform_DisableBoostRecharge();
                    }
                }
            }
            return 0;
        }
        case WM_KEYUP:
        {
            if (wParam == VK_SHIFT) {
                platform_DisableBoost();
                if (!platform_IsBoostRecharging()) {
                    platform_EnableBoostRecharge();
                }
            }
        }
        case WM_TIMER:
        {
            UINT_PTR timer_val = (UINT_PTR)wParam;
            switch (timer_val) {
                case GAME_TIMER_NORMAL_SPEED_ID:
                    if (!platform_IsBoost()) {
                        if (platform_IsStartGame()) {
                            if (platform_IsUpdateScore()) {
                                platform_UpdateScoreDisplay();
                            }
                            platform_GenerateNextFrame(windowHandler.gameFieldWindow);
                        }
                        if (platform_IsGameOver()) {
                            platform_UpdateGameFieldWindow();
                        }
                        platform_RepaintEnergyDisplay();
                    }
                    break;
                case GAME_TIMER_BOOST_ID:
                    if (platform_IsBoost()) {
                        if (platform_IsStartGame()) {
                            if (!platform_IsBoostDepleted()) {
                                if (platform_IsUpdateScore()) {
                                    platform_UpdateScoreDisplay();
                                }
                                platform_GenerateNextFrame(windowHandler.gameFieldWindow);
                            }
                            else {
                                platform_EnableBoostRecharge();
                            }
                        }
                        if (platform_IsGameOver()) {
                            platform_UpdateGameFieldWindow();
                        }
                        platform_UpdateEnergy();
                    }
                    break;
                case GAME_TIMER_BOOST_RECHARGE_ID:
                    if (platform_IsBoostRecharging()) {
                        platform_UpdateEnergy();
                    }
                    break;
            }
            return 0;
        }
        case WM_SIZE:
        {
            if (hwnd == windowHandler.mainWindow) {
                platform_UpdateGameboardWindow();
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


void initializeBrushes() {
    brushHandler.backgroundBrush = CreateSolidBrush(COLOR_SNAKEGAME_BACKGROUND);
    brushHandler.wallBrush = CreateSolidBrush(COLOR_SNAKEGAME_WALL);
    brushHandler.fieldBrush = CreateSolidBrush(COLOR_SNAKEGAME_GAMEFIELD);
    brushHandler.snakeBrush = CreateSolidBrush(COLOR_SNAKEGAME_SNAKE);
    brushHandler.fruitBrush = CreateSolidBrush(COLOR_SNAKEGAME_FRUIT);
}

void win32SetOnCloseCallback(onWindowCloseCallbackFn fn) {
    global_onClose = fn;
}

/*----------------------------------------------------------------------------*/
/*                              Utility                                       */
/*----------------------------------------------------------------------------*/


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
    SelectObject(st->frameDC, platform_GetGameScoreFont());
    DrawText(st->frameDC, platform_GetGameScoreText(), -1, &dataDisplayRect, DT_LEFT | DT_SINGLELINE);
    RestoreDC(st->frameDC, savedDCConfig);

    BitBlt(hdc, 0, 0, st->width, st->height, st->frameDC, 0, 0, SRCCOPY);
    EndPaint(windowHandler.gameDataDisplayWindow, &ps);
}


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

    if (platform_IsGameOver()) {
        int savedDCConfig = SaveDC(st->frameDC);
        SetBkMode(st->frameDC, TRANSPARENT);
        SelectObject(st->frameDC, platform_GetGameScoreFont());

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
    double energyPercentage = (double)(platform_GetGameEnergyLevel() / 100.0);
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
    swprintf(energyValueString, 5, L"%d", platform_GetGameEnergyLevel());

    SelectObject(hdc, platform_GetGameEnergyFont());

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


void drawSnake(HDC hdc) {
    if (platform_IsSnakeNull()) {
        platform_LogErrorMessage("Error in function drawSnake(HDC hdc) of snakeWin32.h.\n\tplatform_IsSnakeNull() == TRUE\n");
    }
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHandler.snakeBrush);
    platform_ResetSnakeNodeIterator();
    RECT snakeCoordRect = platform_GetNodeBoundingRect(platform_GetSnakeNodeIteratorCoord());
    drawCircle(hdc, snakeCoordRect);
    while (platform_IncrementSnakeNodeIterator()) {
        RECT snakeCoordRect = platform_GetNodeBoundingRect(platform_GetSnakeNodeIteratorCoord());
        drawCircle(hdc, snakeCoordRect);
    }
    SelectObject(hdc, oldBrush);
}


void drawGameField(RECT field, HDC hdc) {
    FillRect(hdc, &field, brushHandler.fieldBrush);
}


void drawWalls(HDC hdc) {
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHandler.wallBrush);
    for (int i = 1; i <= GAMEGRIDCOLS; i++) {
        RECT cell_bounds = platform_GetCellBoundingRect((Coord){i, 1});
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);
        cell_bounds = platform_GetCellBoundingRect((Coord){i, GAMEGRIDCOLS});
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);

        cell_bounds = platform_GetCellBoundingRect((Coord){1, i});
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);
        cell_bounds = platform_GetCellBoundingRect((Coord){GAMEGRIDCOLS, i});
        FillRect(hdc, &cell_bounds, brushHandler.wallBrush);
    }
    SelectObject(hdc, oldBrush);
}

void drawFruit(HDC hdc) {
    // --- Fruit body (orange) ---
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushHandler.fruitBrush);

    RECT fruit_bounds = platform_GetNodeBoundingRect(platform_GetFruitCoord());

    Ellipse(hdc, fruit_bounds.left, fruit_bounds.top, fruit_bounds.right, fruit_bounds.bottom);

    // --- Stem ---
    HPEN stemPen = CreatePen(PS_SOLID, 4, RGB(60, 120, 40));
    HPEN oldPen = (HPEN)SelectObject(hdc, stemPen);
    int stem_x = ((fruit_bounds.right - fruit_bounds.left) / 2) + fruit_bounds.left;
    int stem_y = fruit_bounds.top;
    MoveToEx(hdc, stem_x, stem_y, NULL);
    LineTo(hdc, stem_x + 10, stem_y + 10);


    // --- Restore & cleanup ---
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    //DeleteObject(leafBrush);
    DeleteObject(stemPen);
}


void drawCircle(HDC hdc, RECT cell_bounds) {
    Ellipse(hdc, cell_bounds.left, cell_bounds.top, cell_bounds.right, cell_bounds.bottom);
}


void drawDebugGrid(RECT field, HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int cellWidth = platform_GetGameBoardCellWidth();
    int cellHeight = platform_GetGameBoardCellHeight();
    
    for (int i = 1; i <= GAMEGRIDCOLS; i++) {
        int col = field.left + (i * cellWidth);
        MoveToEx(hdc, col, field.top, NULL);
        LineTo(hdc, col, field.bottom);
    }

    for (int i = 1; i <= GAMEGRIDROWS; i++) {
        int row = field.top + (i * cellHeight);
        MoveToEx(hdc, field.left, row, NULL);
        LineTo(hdc, field.right, row);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

/*----------------------------------------------------------------------------*/
/*                              Clean Up                                      */
/*----------------------------------------------------------------------------*/


void windowCleanUp() {
    deleteBrushes();
}


void deleteBrushes() {
    DeleteObject(brushHandler.backgroundBrush);
    DeleteObject(brushHandler.wallBrush);
    DeleteObject(brushHandler.fieldBrush);
    DeleteObject(brushHandler.snakeBrush);
    DeleteObject(brushHandler.fruitBrush);
}
