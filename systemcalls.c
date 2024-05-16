#include <stdio.h>
#include <windows.h>
#include <string.h>

void childProcess(const char *fileName) {
    DWORD childPID = GetCurrentProcessId();
    printf("Child PPID: %lu\n", childPID);

    // Read by child process
    FILE *file = fopen(fileName, "r");
    if (file != NULL) {
        char data[100];
        fgets(data, sizeof(data), file);
        fclose(file);
        printf("Child reading: %s\n", data);
    }

    // Write by child
    FILE *outputFile = fopen(fileName, "a");
    if (outputFile != NULL) {
        fprintf(outputFile, "Child %lu writing...\n", childPID);
        fclose(outputFile);
    }

    // Change permission (Not supported directly in Windows)
}

void parentProcess(const char *fileName) {
    DWORD parentPID = GetCurrentProcessId();
    printf("Parent PID: %lu\n", parentPID);

    // Create child process
    char command[100];
    sprintf(command, "child.exe %s", fileName);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Read from file
    FILE *file = fopen(fileName, "r");
    if (file != NULL) {
        char data[100];
        fgets(data, sizeof(data), file);
        fclose(file);
        printf("Data in file before parent writing: %s\n", data);
    }

    // Write by parent
    FILE *outputFile = fopen(fileName, "a");
    if (outputFile != NULL) {
        fprintf(outputFile, "Parent %lu writing...\n", parentPID);
        fclose(outputFile);
    }

    // Read by parent
    FILE *inputFile = fopen(fileName, "r");
    if (inputFile != NULL) {
        char content[100];
        fread(content, sizeof(char), sizeof(content), inputFile);
        fclose(inputFile);
        printf("Parent %lu read: %s\n", parentPID, content);
    }

    // Change permission (Not supported directly in Windows)
}

int main() {
    const char *fileName = "demo_file.txt";
    FILE *f = fopen(fileName, "w");
    if (f != NULL) {
        fprintf(f, "Initial Content by main\n");
        fclose(f);
    }

    parentProcess(fileName);
    return 0;
}
