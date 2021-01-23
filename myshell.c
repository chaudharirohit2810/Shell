#include "lib.h"
#include "main.h"
#include "utils.h"

int currentpid = -1;

void ctrl_c_handler(int sig) {
    if (currentpid != -1) {
        kill(currentpid, SIGKILL);
        printf("Current process terminated\n");
    }
}

void ctrl_z_handler(int sig) {
    if (currentpid != -1) {
        kill(currentpid, SIGSTOP);
        printf("Current process stopped\n");
    }
}

// To execute multiple pipe commands
void executePipeCommands(char* commands[], int n) {
    int fd[2];       // for main pipe
    int readfd = 0;  // To store the output of previous command

    for (int i = 0; i < n; i++) {
        if (executeChangeDirectory(commands[i]) != -1) {
            continue;  // If the command cd executed successfully
        }

        pipe(fd);
        int pid = fork();
        if (pid == -1) {
            printf("Unable to create child process\n");
            return;
        }
        currentpid = pid;
        if (pid == 0) {
            // Ignore Ctrl + C handler
            signal(SIGINT, SIG_IGN);
            // Ignore Ctrl + Z handler
            signal(SIGTSTP, SIG_IGN);

            // Child Process
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
        } else {
            // Parent process
            wait(0);
            currentpid = -1;
            close(fd[1]);    // Close the write end in parent
            readfd = fd[0];  // Store the output of previous command
        }
    }
}

void startCtrlCHandler() {
    struct sigaction ctrl_c_action;
    ctrl_c_action.sa_handler = ctrl_c_handler;
    sigemptyset(&ctrl_c_action.sa_mask);
    ctrl_c_action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &ctrl_c_action, NULL);
}

void startCtrlZHandler() {
    struct sigaction ctrl_z_action;
    ctrl_z_action.sa_handler = ctrl_z_handler;
    sigemptyset(&ctrl_z_action.sa_mask);
    ctrl_z_action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &ctrl_z_action, NULL);
}

int main() {
    int pid;
    char cmd[512];
    int exitStatus = 0;
    printf("Myshell Usage:\n");
    printf("-----------------------------------------------------\n");
    printf("exit: to exit out of shell\n");
    printf("\n");
    startCtrlCHandler();
    startCtrlZHandler();

    while (1) {
        printf("myshell>");
        scanf("%[^\n]%*c", cmd);

        if (checkExit(cmd) == 0) {  // check for "exit" command
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
