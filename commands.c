#include "lib.h"
#include "utils.h"

// Execute cd command
// Return Values
// 0: if command executed successfully
// -1: if command is not cd
// -2: more than one arguement to cd
// -3: error occured while executing cd
int executeChangeDirectory(char* command) {
    char* args[20];
    char* cmdTemp = malloc(strlen(command) + 1);
    strcpy(cmdTemp, command);
    int res = tokenizeValues(args, cmdTemp);
    if (strcmp("cd", args[0]) != 0) {
        return -1;  // not a CD command
    }
    if (res > 2) {
        printf("cd: too many arguments\n");
        return -2;  // More than one arguement provided to cd
    }
    if (chdir(args[1]) == -1) {
        perror("Error");
        return -3;  // error occured while executing cd
    }
    return 0;
}