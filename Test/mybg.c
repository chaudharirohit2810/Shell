#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int isBackground(char* command, char* bgProcesses[]) {
    char* cmdTemp = malloc(strlen(command) + 1);
    strcpy(cmdTemp, command);
    char* token = strtok(cmdTemp, "&");
    if (strcmp(token, command) == 0) {
        return -1;
    }
    int i = 0;
    while (token) {
        bgProcesses[i] = malloc(strlen(token) + 1);
        strcpy(bgProcesses[i], token);
        token = strtok(NULL, "&");
        i++;
    }
    return i;
}

int main() {
    char* cmd = "ls -l -a & firefox & grep -c google.com | grep rtt & ls -l &";
    int len = strlen(cmd);
    printf("%c\n", cmd[len - 1]);
    char* bgProcesses[20];
    int num = isBackground(cmd, bgProcesses);
    for (int i = 0; i < num; i++) {
        printf("%s\n", bgProcesses[i]);
    }
    return 0;
}