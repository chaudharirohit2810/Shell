#include "lib.h"

// Utility function to separate different arguements
void tokenizeValues(char* args[], char cmd[]) {
    char* token = strtok(cmd, " ");
    int i = 0;
    while (token) {
        args[i] = malloc(strlen(token) + 1);
        strcpy(args[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
}

// To exit out of shell
int checkExit(char* cmd) {
    if (strcmp(cmd, "exit") == 0) {
        printf("bye\n");
        return -1;
    }
    return 0;
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