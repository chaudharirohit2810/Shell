#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

        char* args[20];  // Store arguements
        tokenizeValues(args, cmd);

        if (strcmp(args[0], "exit") == 0) {
            printf("bye\n");
            exitStatus = 1;
            exit(0);
        }

        pid = fork();
        if (pid == 0) {
            // Child Process

            // Tokenize values based on space
            if (execve(args[0], args, NULL) == -1) {
                exit(0);
            }
        } else {
            // Parent Process
            wait(0);
        }
    }
    return 0;
}
