#include "lib.h"
#include "main.h"
#include "utils.h"

// To execute multiple pipe commands
// commands: Commands that are piped one after another
// n: Number of piped commands
// isBackground: Whether to run the command in background or foreground
void executePipeCommands(char* commands[], int n, int isBackground) {
    int fd[2];       // for main pipe
    int readfd = 0;  // To store the output of previous command

    for (int i = 0; i < n; i++) {
        if (executeChangeDirectory(commands[i]) != -1) {
            continue;  // If the command cd executed successfully
        }

        pipe(fd);
        int pid = fork();
        if (pid == -1) {
            printf("rsh: Unable to create child process\n");
            return;
        }
        if (isBackground == 0) {
            setCurrentpid(pid);
            setCurrentCommand(commands[i]);
        } else {
            addStoppedProcess(pid, commands[i]);
        }

        if (pid == 0) {  // Child Process
            // Ignore Ctrl + Z and Ctrl + C signals in child
            ignoreSignals();

            if (i != 0) {                    // First Command
                dup2(readfd, STDIN_FILENO);  // If the command is not the first command then give standard output of previous as input
            }
            if (i != n - 1) {                // Last Command
                dup2(fd[1], STDOUT_FILENO);  // If the command is not the last command then store the standard output
            }

            close(fd[0]);  // Close the read end in child

            if (executeRedirection(commands[i]) == 0) {  // Check redirection
                exit(0);
            }

            if (executeCommand(commands[i]) == -1) {  // Execute normal command
                close(fd[1]);
                close(readfd);
                exit(0);
            }

        } else {                      // Parent process
            if (isBackground == 0) {  // wait only if process is not required to run in background
                waitpid(-1, NULL, WUNTRACED);
            }
            close(fd[1]);    // Close the write end in parent
            readfd = fd[0];  // Store the output of previous command
        }
    }
}

// To tokenize the pipe commands
int tokenizePipeCommands(char* cmd, char* pipeCommands[]) {
    int i = 0;
    char* token = strtok(cmd, "|");
    while (token) {
        /* code */
        pipeCommands[i] = malloc(strlen(token) + 1);
        strcpy(pipeCommands[i], token);
        token = strtok(NULL, "|");
        i++;
    }
    return i;
}

// Wrapper to execute piped commands
void pipeWrapper(char* command, int isBackground) {
    int i = -1;
    do {
        // To handle the condition of | after &
        i++;
        if (command[i] == '|') {
            printf("rsh: syntax error near unexpected token `|'\n");
            return;
        }
    } while (command[i] == ' ');

    char* pipeCommands[20];
    int pipeCommandsCount = tokenizePipeCommands(command, pipeCommands);
    executePipeCommands(pipeCommands, pipeCommandsCount, isBackground);
}

int main() {
    int pid;
    char cmd[512];
    int exitStatus = 0;
    printf("\nrsh Usage:\n");
    printf("-----------------------------------------------------\n");
    printf("exit: to exit out of shell\n");
    printf("\n");
    startCtrlCHandler();
    startCtrlZHandler();

    while (1) {
        printf("rsh>");
        scanf("%[^\n]%*c", cmd);

        if (checkExit(cmd) == 0) {  // check for "exit" command
            return 0;
        }

        if (strcmp(cmd, "bg") == 0) {
            executeBg();
            continue;
        }

        if (strcmp(cmd, "fg") == 0) {
            executeFg();
            continue;
        }

        char* bgProcesses[20];
        int isForeground = 1;
        int res = isBackground(cmd, bgProcesses);  // Get all the processes that need to be started in background
        if (res != -1) {
            for (int i = 0; i < res - 1; i++) {
                pipeWrapper(bgProcesses[i], 1);  // Start the background processes
            }

            int cmdLength = strlen(cmd);
            int isLastBackground = ('&' == cmd[cmdLength - 1]) ? 1 : 0;

            if (isLastBackground == 1) {
                pipeWrapper(bgProcesses[res - 1], 1);
            } else {
                pipeWrapper(bgProcesses[res - 1], 0);
            }
        } else {
            pipeWrapper(cmd, 0);
        }
    }
    return 0;
}
