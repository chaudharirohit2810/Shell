#include "lib.h"
#include "utils.h"

// Execute the output redirection command
int executeOutputRedirection(char* command, char* filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        return -1;  // Cannot create file so return errorcode
    }

    int pid = fork();
    if (pid == 0) {
        dup2(fd, STDOUT_FILENO);  // To store output in create file
        close(fd);
        if (executeCommand(command) == -1) {
            close(fd);
            exit(0);  // Something went wrong while executing process
        }
    } else {
        close(fd);
        wait(0);
    }
    return 0;  // Command got executed successfully
}

// Separate command and filename from output redirection
int decodeOutputRedirection(char* command, char* args[]) {
    char* tempCommand = malloc(strlen(command) + 1);
    strcpy(tempCommand, command);
    char* token = strtok(command, ">");
    if (strcmp(token, tempCommand) == 0) {
        return -1;  // Command does not contain output redirection
    }

    int i = 0;
    while (token != NULL) {
        args[i] = malloc(strlen(token) + 1);
        strcpy(args[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    return i;  // Return the number of arguements
}

int checkOutputRedirection(char* command) {
    char* redirectionArgs[20];
    if (decodeOutputRedirection(command, redirectionArgs) == -1) {
        return -1;  // Command does not contain output redirection
    }
    if (executeOutputRedirection(redirectionArgs[0], redirectionArgs[1]) == -1) {
        return -1;  // Something went wrong file while executing redirection
    }
    return 0;
}