#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

// To detect and extract to commands in pipe
int extractPipeCommands(char* commands[], char cmd[]) {
    char* cmdTemp = malloc(strlen(cmd) + 1);
    strcpy(cmdTemp, cmd);

    char* token = strtok(cmd, "|");
    if (strcmp(token, cmdTemp) == 0) {
        // No pipes exist
        return -1;
    }
    commands[0] = malloc(strlen(token) + 1);
    strcpy(commands[0], token);
    token = strtok(NULL, "|");
    commands[1] = malloc(strlen(token) + 1);
    strcpy(commands[1], token);
}

void mypipe(char* pipeCommands[2], int mainfd[]) {
    char* cmd1[20];
    tokenizeValues(cmd1, pipeCommands[0]);
    char* cmd2[20];
    tokenizeValues(cmd2, pipeCommands[1]);

    int fd[2];
    pipe(fd);

    int pid1 = fork();
    if (pid1 == 0) {
        // Child Process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        if (execvp(cmd1[0], cmd1) == -1) {
            exit(0);
        }
    }
    int fd1[2];
    int pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        dup2(fd1[1], STDOUT_FILENO);
        close(fd1[0]);
        close(fd1[1]);
        close(fd[1]);
        close(fd[0]);
        if (execvp(cmd2[0], cmd2) == -1) {
            exit(0);
        }
    }

    close(fd[1]);
    close(fd[0]);
    close(fd1[1]);
    close(fd1[0]);
    mainfd = fd1;

    waitpid(pid1, 0, 0);
    waitpid(pid2, 0, 0);
}

// Function to check to exit out of shell using "exit" command
void checkExit(char* cmd) {
    char* args[20];  // To store arguements
    char* cmdTemp = malloc(sizeof(cmd) + 1);
    strcpy(cmdTemp, cmd);  // Copy to a temporary string so that original string does not get affected
    tokenizeValues(args, cmdTemp);

    if (strcmp(args[0], "exit") == 0) {
        printf("bye\n");
        exit(0);
    }
}

char* pipeExtract() {
    return strtok(NULL, "|");
}

int main() {
    int pid;
    char cmd[512];
    int exitStatus = 0;
    printf("Myshell Usage:\n");
    printf("-----------------------------------------------------\n");
    printf("exit: to exit out of shell\n");
    printf("\n");
    while (1) {
        printf("myshell>");
        scanf("%[^\n]%*c", cmd);

        pid = fork();
        if (pid == 0) {
            // Child Process
            char* pipeCommands[2];
            int res = extractPipeCommands(pipeCommands, cmd);
            if (res == -1) {
                // No pipe exist
                char* cmdargs[20];
                tokenizeValues(cmdargs, cmd);
                if (execvp(cmdargs[0], cmdargs) == -1) {
                    exit(0);
                }
            } else {
                // Atleast one pipe exists
                // TODO: Handle Multiple pipes
                int newfd[2];
                mypipe(pipeCommands, newfd);
            }
            char* cmdTemp = malloc(strlen(cmd) + 1);
            strcpy(cmdTemp, cmd);

        } else {
            // Parent Process
            wait(0);
        }
    }
    return 0;
}
