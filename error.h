#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

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
    if (remove(filename) != 0) {
        wchar_t* errMsg = (wchar_t*)malloc(sizeof(char) * 40);
        swprintf(errMsg, L"Error deleting file \"", "%s\"\n", filename);
        logError(errMsg);
    }
}

void wipeErrorLog() {
    deleteLogFile("error_log.txt");
}

void wipeDebugLog() {
    deleteLogFile("debug_log.txt");
}

#endif