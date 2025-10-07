#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

// ******************** Function Prototypes ********************
void logError(const wchar_t * message);
void logDebugMessage(const wchar_t * message);
void deleteLogFile(const char *filename);
void wipeErrorLog();
void wipeDebugLog();
void resetLogs();
/* ************************************************************ */

// ******************** Function Implementations ********************
void logError(const wchar_t * message) {
    FILE *fp = fopen("error_log.txt", "w");
    if (fp == NULL) {
        perror("Error creating error log file.");
        return;
    }
    fprintf(fp, "%ls", message);
    fclose(fp);
    exit(1);
}

void logDebugMessage(const wchar_t * message) {
    FILE *fp = fopen("debug_log.txt", "a");
    if (fp == NULL) {
        perror("Error creating debug log file.");
        return;
    }
    fprintf(fp, "%ls", message);
    fclose(fp);
}

void deleteLogFile(const char *filename) {
    int removeVal = remove(filename);
    if (!(removeVal == 0 || removeVal == -1)) {
        wchar_t errMsg[256];
        swprintf(errMsg, 256, L"remove() return value: %d - Error deleting file \"%hs\"\n", removeVal, filename);
        logError(errMsg);
    }
}

void wipeErrorLog() {
    deleteLogFile("error_log.txt");
}

void wipeDebugLog() {
    deleteLogFile("debug_log.txt");
}

void resetLogs() {
    wipeDebugLog();
    wipeErrorLog();
}
/* ************************************************************ */

#endif