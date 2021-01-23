#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ctrl_c_handler(int sig) {
    printf("Ctrl + C was pressed\n");
    exit(0);
}

void ctrl_z_handler(int sig) {
    printf("Ctrl + Z was pressed\n");
    exit(0);
}

int main() {
    signal(SIGINT, ctrl_c_handler);
    signal(SIGTSTP, ctrl_z_handler);
    while (1)
        ;
    return 0;
}