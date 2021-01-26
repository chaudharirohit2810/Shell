#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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
    return i;
}

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

int executeOutputRedirection(char* command, char* filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        printf("file does not exist");
        return -1;
    }

    char* args[20];
    tokenizeValues(args, command);
    int pid = fork();
    if (pid == 0) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
        if (execvp(args[0], args) == -1) {
            perror("rsh");
            exit(0);
        }
    } else {
        close(fd);
        wait(0);
    }
}

int main() {
    char command[100] = "ls -a > rohit.txt";
    char* redirectionArgs[20];
    int res = decodeOutputRedirection(command, redirectionArgs);
    if (res == -1) {
        printf("Redirection does not exist\n");
        return -1;
    }
    executeOutputRedirection(redirectionArgs[0], redirectionArgs[1]);

    return 0;
}