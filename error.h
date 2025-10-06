#ifndef ERROR_H
#define ERROR_H

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

int deleteFile(const char *filename) {
    if (remove(filename) == 0) {
        return 1; // success
    } else {
        perror("Error deleting file");
        return 0; // failure
    }
}

void wipeErrorLog() {
    deleteFile("error_log.txt");
}

void wipeDebugLog() {
    deleteFile("debug_log.txt");
}

#endif