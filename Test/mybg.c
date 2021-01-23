#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int pid = fork();
    if (pid == -1) {
        printf("Failed to create child process");
        return -1;
    }

    if (pid == 0) {
        while (1) {
            printf("Hello world!\n");
            usleep(50000);
        }
    } else {
        kill(pid, SIGSTOP);

        sleep(10);
        kill(pid, SIGCONT);
        sleep(1);
        kill(pid, SIGKILL);
        wait(0);
    }
    return 0;
}