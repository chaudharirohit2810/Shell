#include "lib.h"
#include "main.h"
#include "utils.h"

// To execute multiple pipe commands
void executePipeCommands(char* commands[], int n) {
    int fd[2];       // for main pipe
    int readfd = 0;  // To store the output of previous command

    for (int i = 0; i < n; i++) {
        pipe(fd);
        int pid = fork();
        if (pid == 0) {
            // Child Process
            if (i != 0) {
                dup2(readfd, STDIN_FILENO);  // If the command is not the first command then give standard output of previous as input
            }
            if (i != n - 1) {
                dup2(fd[1], STDOUT_FILENO);  // If the command is not the last command then store the standard output
            }

            close(fd[0]);  // Close the read end in child

            if (checkOutputRedirection(commands[i]) != -1) {
                exit(0);
            }

            if (executeCommand(commands[i]) == -1) {  // To check if error has occured
                close(fd[1]);
                close(readfd);
                exit(0);
            }
        } else {
            // Parent process
            wait(0);
            close(fd[1]);    // Close the write end in parent
            readfd = fd[0];  // Store the output of previous command
        }
    }
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

        if (checkExit(cmd) == -1) {
            return 0;
        }

        char* pipeCommands[20];
        int i = 0;
        char* token = strtok(cmd, "|");
        while (token) {
            /* code */
            pipeCommands[i] = malloc(strlen(token) + 1);
            strcpy(pipeCommands[i], token);
            token = strtok(NULL, "|");
            i++;
        }
        executePipeCommands(pipeCommands, i);
    }
    return 0;
}
