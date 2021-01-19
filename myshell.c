#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>


int main() {
    int pid;
    char cmd[512];
    while (1)
    {
        printf("myshell>");
        scanf("%[^\n]%*c", cmd);
        
        pid = fork();
        if(pid == 0) {
            // printf("%s", cmd);
            char* mainCommand;
            char* token = strtok(cmd, " ");
            int i = 0;
            char* args[20];            
            while (token)
            {
                if(i == 0) {
                    mainCommand = token;
                }
                args[i] = malloc(strlen(token) + 1);
                strcpy(args[i], token);
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL;

            execve(mainCommand, args, NULL);
            
            // // Child Process
            // if(execl(mainCommand, mainCommand, args[0], args[1], args[2], NULL) == -1) {
            //     exit(0);
            // }
        }else {
            // Parent Process
            wait(0);
        }
    }
    return 0;
}