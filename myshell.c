#include "lib.h"
#include "main.h"
#include "utils.h"
#define DEFAULT_PID 0

int currentpid = DEFAULT_PID;
int stoppedpid = DEFAULT_PID;

void ctrl_c_handler(int sig) {
    if (currentpid != DEFAULT_PID) {
        if (kill(currentpid, SIGKILL) == -1) {  // Kill current process
            perror("Error:");
            return;
        }
        printf("Process with pid %d terminated\n", currentpid);
        currentpid = DEFAULT_PID;
    }
}

void ctrl_z_handler(int sig) {
    if (currentpid != DEFAULT_PID) {            // Only if particular process is running
        if (kill(currentpid, SIGSTOP) == -1) {  // Stop current process
            perror("Error:");
            currentpid = DEFAULT_PID;
            return;
        }
        printf("Process with pid %d Stopped\n", currentpid);
        stoppedpid = currentpid;  // For handling of bg and fg
        currentpid = DEFAULT_PID;
    }
}

// To handle "fg" command
void continue_process_in_fg() {
    if (stoppedpid != DEFAULT_PID) {
        if (kill(stoppedpid, SIGCONT) == -1) {  // Continue the stopped process
            perror("Error:");
            stoppedpid = DEFAULT_PID;
            return;
        }
        currentpid = stoppedpid;  // When the process is started in foreground again set the currentpid for Ctrl+C handling
        printf("Process with pid %d continued\n", stoppedpid);
        waitpid(stoppedpid, NULL, WUNTRACED);  // Wait for process to end as process is in forefround
    } else {
        printf("No stopped process exists\n");
    }
}

// To handle "bg" command
void continue_process_in_bg() {
    if (stoppedpid != DEFAULT_PID) {
        if (kill(stoppedpid, SIGCONT) == -1) {  // Continue the stopped process
            perror("Error:");
            stoppedpid = DEFAULT_PID;
            return;
        }
        // Set the pids to default
        stoppedpid = DEFAULT_PID;
        currentpid = DEFAULT_PID;
        printf("Process with pid %d continued\n", stoppedpid);

    } else {
        printf("No stopped process exists\n");
    }
}

void startCtrlZHandler() {
    struct sigaction ctrl_z_action;
    ctrl_z_action.sa_handler = ctrl_z_handler;
    sigemptyset(&ctrl_z_action.sa_mask);
    ctrl_z_action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &ctrl_z_action, NULL);
}

void startCtrlCHandler() {
    struct sigaction ctrl_c_action;
    ctrl_c_action.sa_handler = ctrl_c_handler;
    sigemptyset(&ctrl_c_action.sa_mask);
    ctrl_c_action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &ctrl_c_action, NULL);
}

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
            printf("Unable to create child process\n");
            return;
        }

        currentpid = pid;
        if (pid == 0) {
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
            if (isBackground == 0) {
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
            printf("myshell: syntax error near unexpected token `|'\n");
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

        if (strcmp(cmd, "bg") == 0) {
            continue_process_in_bg();
            continue;
        }

        if (strcmp(cmd, "fg") == 0) {
            continue_process_in_fg();
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
