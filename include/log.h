#ifndef LOG_H
#define LOG_H

#include <wchar.h>   // wchar_t, swprintf
#include <stdlib.h>  // exit
#include <stdio.h>   // FILE, fopen, fprintf, perror

void logError(const wchar_t* message);
void logDebugMessage(const wchar_t* message);
void deleteLogFile(const char* filename);
void wipeErrorLog(void);
void wipeDebugLog(void);
void resetLogs(void);

#endif