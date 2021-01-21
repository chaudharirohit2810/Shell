#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void mypipe(char* command1[20], char* command2[20]) {
    int fd[2];
    pipe(fd);

    int pid1 = fork();
    if (pid1 == 0) {
        // Child Process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        if (execvp(command1[0], command1) == -1) {
            exit(0);
        }
    }

    int pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        if (execvp(command2[0], command2) == -1) {
            exit(0);
        }
    }

    close(fd[1]);
    close(fd[0]);

    waitpid(pid1, 0, 0);
    waitpid(pid2, 0, 0);
}

int main() {
    printf("Started piping\n");
    char* command1[20] = {"ping", "-c", "5", "google.com", NULL};
    char* command2[20] = {"grep", "rtt", NULL};
    mypipe(command1, command2);
    return 0;
}