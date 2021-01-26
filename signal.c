#include "lib.h"

int currentpid = DEFAULT_PID;
char* currentCommand = NULL;

typedef struct stoppedProcess {
    char* command;
    int pid;
} stoppedProcess;

stoppedProcess* stoppedProcesses[100];

int stoppedProcessesCount = 0;
int bgIndex = 0;

void addStoppedProcess(int pid, char* cmd) {
    stoppedProcess* process = (stoppedProcess*)malloc(sizeof(stoppedProcess));
    process->command = cmd;
    process->pid = pid;
    stoppedProcesses[stoppedProcessesCount] = process;
    stoppedProcessesCount++;
    bgIndex++;
}

// Setter for currentpid
void setCurrentpid(int pid) {
    currentpid = pid;
}

void setCurrentCommand(char* cmd) {
    if (cmd == NULL) {
        currentCommand = NULL;
        return;
    }
    currentCommand = malloc(strlen(cmd) + 1);
    strcpy(currentCommand, cmd);
}

// -------------------------------------------------------------------Ctrl + C Handler--------------------------------------------------------------------------
// Signal handler for SIGINT
void ctrl_c_handler(int sig) {
    if (currentpid != DEFAULT_PID) {
        if (kill(currentpid, SIGKILL) == -1) {  // Kill current process
            perror("rsh");
            return;
        }
        stoppedProcessesCount--;
        bgIndex--;
        printf("\npid:%d\tkilled\t\t\t%s\n", currentpid, currentCommand);
        setCurrentpid(DEFAULT_PID);
        setCurrentCommand(NULL);
    }
}

// To handle SIGINT in parent
void startCtrlCHandler() {
    struct sigaction ctrl_c_action;
    ctrl_c_action.sa_handler = ctrl_c_handler;
    sigemptyset(&ctrl_c_action.sa_mask);
    ctrl_c_action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &ctrl_c_action, NULL);
}

//------------------------------------------------------------------------Ctrl+Z handler--------------------------------------------------------------------------

// Signal handler for SIGTSTP
void ctrl_z_handler(int sig) {
    if (currentpid != DEFAULT_PID) {            // Only if particular process is running
        if (kill(currentpid, SIGSTOP) == -1) {  // Stop current process
            perror("rsh");
            currentpid = DEFAULT_PID;
            return;
        }
        int flag = 0;
        for (int i = 0; i < stoppedProcessesCount; i++) {
            if (stoppedProcesses[i]->pid == currentpid) {
                flag = 1;
            }
        }
        if (flag != 1) {
            addStoppedProcess(currentpid, currentCommand);
        }
        int printIndex = flag == 0 ? stoppedProcessesCount : bgIndex;
        printf("\n[%d]+\tStopped\t\t\t%s [pid: %d]\n", printIndex, currentCommand, currentpid);
    }
}

// To handle SIGTSTP in parent
void startCtrlZHandler() {
    struct sigaction ctrl_z_action;
    ctrl_z_action.sa_handler = ctrl_z_handler;
    sigemptyset(&ctrl_z_action.sa_mask);
    ctrl_z_action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &ctrl_z_action, NULL);
}

// ------------------------------------------------------------------bg & fg commands-----------------------------------------------------------------------------

// To execute "fg" command
void executeFg() {
    if (bgIndex == 0 && stoppedProcessesCount != 0) {
        bgIndex = stoppedProcessesCount;
    }
    if (bgIndex != 0) {
        int pid = stoppedProcesses[bgIndex - 1]->pid;
        char* command = stoppedProcesses[bgIndex - 1]->command;
        printf("%s\n", command);
        if (kill(pid, SIGCONT) == -1) {  // Continue the stopped process
            perror("rsh");
            return;
        }
        // When the process is started in foreground again set the current pid and command values  for Ctrl+C handling
        setCurrentpid(pid);
        setCurrentCommand(command);

        waitpid(pid, NULL, WUNTRACED);  // Wait for process to end as process is in forefround
    } else {
        printf("rsh: No process available\n");
    }
}

// To execute "bg" command
void executeBg() {
    if (bgIndex != 0) {
        int pid = stoppedProcesses[bgIndex - 1]->pid;
        char* command = stoppedProcesses[bgIndex - 1]->command;
        printf("%s &\n", command);
        if (kill(pid, SIGCONT) == -1) {  // Continue the stopped process
            perror("rsh");
            return;
        }
        bgIndex--;
    } else {
        printf("rsh: No process available\n");
    }
}