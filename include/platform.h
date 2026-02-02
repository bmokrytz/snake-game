#ifndef PLATFORM_H
#define PLATFORM_H

#include <windows.h>
#include "snakeWin32.h"

// Lifecycle
BOOL platformInit(HINSTANCE hInstance, int nCmdShow);
void platformShutdown(void);

// Rendering
void platformRepaint(void);

// Logs
void platformResetLogs(void);

// Input → game hooks
void platformSetDirection(int direction);
void platformTogglePause(void);
void platformSetBoost(void);
void platformDisableBoost(void);

#endif
