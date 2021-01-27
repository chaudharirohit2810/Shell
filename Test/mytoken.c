#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility function to separate different arguements
int tokenizeValues(char* args[], char* cmd) {
    char* cmdTemp = malloc(strlen(cmd) + 1);
    strcpy(cmdTemp, cmd);
    char* token = strtok(cmdTemp, " ");
    int i = 0;
    while (token) {
        if (token[0] == '\"') {
            args[i] = malloc(512 * sizeof(char));
            strcpy(args[i], token);
            int flag = 1;
            // args[i] = token;
            while (token != NULL && token[strlen(token) - 1] != '\"') {
                /* code */
                if (flag != 1) {
                    strcat(args[i], " ");
                    strcat(args[i], token);
                }
                flag = 0;
                token = strtok(NULL, " ");
            }
            // printf("%s\n", args[i]);
            if (token != NULL) {
                strcat(args[i], " ");
                strcat(args[i], token);
            }
            token = strtok(NULL, " ");
        } else {
            args[i] = malloc(strlen(token) + 1);
            strcpy(args[i], token);
            token = strtok(NULL, " ");
        }
        i++;
    }
    args[i] = NULL;
    return i;
}

int main() {
    char* args[20];
    char cmd[512];
    scanf("%[^\n]%*c", cmd);
    int n = tokenizeValues(args, cmd);
    printf("Args:\n");
    for (int i = 0; i < n; i++) {
        printf("%s\n", args[i]);
    }
    return 0;
}