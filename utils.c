#include "lib.h"

// Utility function to separate different arguements
int tokenizeValues(char* args[], char* cmd) {
    char* cmdTemp = malloc(strlen(cmd) + 1);
    strcpy(cmdTemp, cmd);
    char* token = strtok(cmdTemp, " ");
    int i = 0;
    while (token) {
        // To separate args with quotes (2nd condition is to check if only one word is preset in arg)
        if (token[0] == '\"' && token[strlen(token) - 1] != '\"') {
            args[i] = malloc(512);  // TODO: Find alternative so we don't need to malloc fix size
            token = token + 1;
            strcpy(args[i], token);
            int flag = 1;

            while (token != NULL && token[strlen(token) - 1] != '\"') {
                if (flag != 1) {  // Do not copy the first word again
                    strcat(args[i], " ");
                    strcat(args[i], token);
                }
                flag = 0;
                token = strtok(NULL, " ");
            }

            if (token != NULL) {  // If token is not null then its a terminating word
                strcat(args[i], " ");
                strcat(args[i], token);
            } else {
                return -1;  //quotes are not closed
            }
            args[i][strlen(args[i]) - 1] = '\0';
            token = strtok(NULL, " ");

        } else {
            // For args without quotes

            args[i] = malloc(strlen(token) + 1);
            strcpy(args[i], token);
            token = strtok(NULL, " ");
        }
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
    int res = tokenizeValues(args, command);
    if (res == -1) {
        printf("rsh: invalid arguements");
        return -1;
    }
    if (execvp(args[0], args) == -1) {
        perror("rsh");
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

// To ignore Ctrl + Z and Ctrl + C signals for child
void ignoreSignals() {
    struct sigaction childsigact;
    childsigact.sa_handler = SIG_IGN;
    childsigact.sa_flags = SA_RESTART;
    sigaction(SIGINT, &childsigact, NULL);
    sigaction(SIGTSTP, &childsigact, NULL);
}