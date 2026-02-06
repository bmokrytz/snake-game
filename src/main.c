

#include <stdio.h>
#include <windows.h>
#include "platform.h"



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)pCmdLine;

    platform_ResetLogs();

    if (!platform_Init(hInstance, nCmdShow)) {
        // If you want, logError(L"platform_Init failed\n");
        return 1;
    }

    // platform_Init returns after WM_QUIT / shutdown.
    return 0;
}
