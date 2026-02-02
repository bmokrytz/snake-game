
#include "platform.h"
#include "log.h"

/* ===============================
   Lifecycle
================================== */
BOOL platformInit(HINSTANCE hInstance, int nCmdShow)
{
    // --- Create windows, brushes, fonts, handlers ---
    windowSetup(hInstance);

    if (!windowHandler.mainWindow) {
        return FALSE;
    }

    ShowWindow(windowHandler.mainWindow, nCmdShow);
    UpdateWindow(windowHandler.mainWindow);

    // --- Start Win32 message loop ---
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

void platformShutdown(void)
{
    windowCleanUp();
    gameShutdown();
}

void platformShutdown(void);

// Rendering
void platformRepaint(void);

// Logs
void platformResetLogs(void) {
    resetLogs();
}

// Input → game hooks
void platformSetDirection(int direction);
void platformTogglePause(void);
void platformSetBoost(void);
void platformDisableBoost(void);