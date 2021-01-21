#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Utility function to separate different arguements
void tokenizeValues(char* args[], char* cmd) {
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

void multiplePipes(char* commands[], int n) {
    int fd[2];
    int readfd = 0;

    for (int i = 0; i < n; i++) {
        pipe(fd);
        int pid = fork();
        if (pid == 0) {
            if (i != 0) {
                dup2(readfd, STDIN_FILENO);
            }
            if (i != n - 1) {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            char* args[20];
            tokenizeValues(args, commands[i]);
            execvp(args[0], args);
            exit(0);
        } else {
            wait(0);
            close(fd[1]);
            readfd = fd[0];
        }
    }
}

int main() {
    char* commands[3];
    char* cmd1 = "echo \"this line contains rtt\"\n\"another line \"";
    commands[0] = malloc(strlen(cmd1) + 1);

    strcpy(commands[0], cmd1);
    char* cmd2 = "grep a";
    commands[1] = malloc(strlen(cmd2) + 1);
    strcpy(commands[1], cmd2);

    char* cmd3 = "less";
    commands[2] = malloc(strlen(cmd3) + 1);
    strcpy(commands[2], cmd3);

    multiplePipes(commands, 3);
    return 0;
}