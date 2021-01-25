#include "lib.h"

int currentpid = DEFAULT_PID;
int stoppedpid = DEFAULT_PID;

// Setter for currentpid
void setCurrentpid(int pid) {
    currentpid = pid;
}

// -------------------------------------------------------------------Ctrl + C Handler--------------------------------------------------------------------------

void ctrl_c_handler(int sig) {
    if (currentpid != DEFAULT_PID) {
        if (kill(currentpid, SIGKILL) == -1) {  // Kill current process
            perror("Error");
            return;
        }
        printf("Process with pid %d terminated\n", currentpid);
        currentpid = DEFAULT_PID;
        stoppedpid = DEFAULT_PID;
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
            perror("Error");
            currentpid = DEFAULT_PID;
            return;
        }
        printf("Process with pid %d Stopped\n", currentpid);
        stoppedpid = currentpid;  // For handling of bg and fg
        currentpid = DEFAULT_PID;
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
    if (stoppedpid != DEFAULT_PID) {
        if (kill(stoppedpid, SIGCONT) == -1) {  // Continue the stopped process
            perror("Error");
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

// To execute "bg" command
void executeBg() {
    if (stoppedpid != DEFAULT_PID) {
        if (kill(stoppedpid, SIGCONT) == -1) {  // Continue the stopped process
            perror("Error");
            stoppedpid = DEFAULT_PID;
            return;
        }
        printf("Process with pid %d continued\n", stoppedpid);
        // Set the pids to default
        stoppedpid = DEFAULT_PID;
    } else {
        printf("No stopped process exists\n");
    }
}