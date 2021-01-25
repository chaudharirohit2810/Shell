#include "lib.h"

// Utility function to separate different arguements
int tokenizeValues(char* args[], char cmd[]) {
    char* token = strtok(cmd, " ");
    int i = 0;
    while (token) {
        args[i] = malloc(strlen(token) + 1);
        strcpy(args[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
    return i;
}

// To exit out of shell
int checkExit(char* cmd) {
    if (strcmp(cmd, "exit") == 0) {
        printf("bye\n");
        return 0;
    }
    return -1;
}

// Utility function to execute command
int executeCommand(char* command) {
    char* args[20];
    tokenizeValues(args, command);
    if (execvp(args[0], args) == -1) {
        perror("Error");
        return -1;  // Return -1 if error occurs
    }
    return 0;
}

// Separate all the backgroung processes
int isBackground(char* command, char* bgProcesses[]) {
    char* cmdTemp = malloc(strlen(command) + 1);
    strcpy(cmdTemp, command);
    char* token = strtok(cmdTemp, "&");
    if (strcmp(token, command) == 0) {
        return -1;
    }
    int i = 0;
    while (token) {
        bgProcesses[i] = malloc(strlen(token) + 1);
        strcpy(bgProcesses[i], token);
        token = strtok(NULL, "&");
        i++;
    }
    return i;
}