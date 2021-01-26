#include "lib.h"
#include "utils.h"

// ------------------------------------------------------------------------------Output Redirection-----------------------------------------------------------------

// Execute the output redirection command
int executeOutputRedirection(char* command, char* filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        return -1;  // Cannot create a file so return errorcode
    }

    dup2(fd, STDOUT_FILENO);  // To store output in create file
    close(fd);
    if (executeCommand(command) == -1) {
        return -1;  // Something went wrong while executing process
    }
    return 0;  // Command got executed successfully
}

// Separate command and filename from output redirection
int decodeOutputRedirection(char* command, char* args[]) {
    char* tempCommand = malloc(strlen(command) + 1);
    strcpy(tempCommand, command);
    char* token = strtok(command, ">");
    if (strcmp(token, tempCommand) == 0) {
        return -1;  // Command does not contain output redirection
    }

    int i = 0;
    while (token != NULL) {
        args[i] = malloc(strlen(token) + 1);
        strcpy(args[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    return i;  // Return the number of arguements
}

// Combine decode and execute of output redirection
int checkOutputRedirection(char* command) {
    char* redirectionArgs[20];
    int res = decodeOutputRedirection(command, redirectionArgs);
    if (res == -1) {
        return -1;  // Command does not contain output redirection
    }
    char* temp = redirectionArgs[0];
    for (int i = 2; i < res; i++) {  // Add additonal arguements after redirection operator in main command
        strcat(temp, redirectionArgs[i]);
        strcat(temp, " ");
    }
    if (executeOutputRedirection(temp, redirectionArgs[1]) == -1) {
        return -1;  // Something went wrong file while executing redirection
    }
    return 0;
}

// ------------------------------------------------------------------------------Input Redirection-----------------------------------------------------------------

// Execute the input redirection command
int executeInputRedirection(char* command, char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return -1;  // Cannot read or find file
    }

    dup2(fd, STDIN_FILENO);  // Pass the file as standard input
    close(fd);
    if (executeCommand(command) == -1) {
        return -1;  // Something went wrong while executing process
    }
    return 0;  // Command got executed successfully
}

// Separate command and filename from input redirection
int decodeInputRedirection(char* command, char* args[]) {
    char* tempCommand = malloc(strlen(command) + 1);
    strcpy(tempCommand, command);
    char* token = strtok(command, "<");
    if (strcmp(token, tempCommand) == 0) {
        return -1;  // Command does not contain input redirection
    }

    int i = 0;
    while (token != NULL) {
        args[i] = malloc(strlen(token) + 1);
        strcpy(args[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    return i;  // Return the number of arguements
}

// Combine decode and execute of input redirection
int checkInputRedirection(char* command) {
    char* redirectionArgs[20];
    int res = decodeInputRedirection(command, redirectionArgs);
    if (res == -1) {
        return -1;  // Command does not contain input redirection
    }
    char* temp = redirectionArgs[0];
    for (int i = 2; i < res; i++) {  // Add additonal arguements after redirection operator in main command
        strcat(temp, redirectionArgs[i]);
        strcat(temp, " ");
    }

    if (executeInputRedirection(redirectionArgs[0], redirectionArgs[1]) == -1) {
        return -1;  // Something went wrong file while executing redirection
    }
    return 0;
}

// ---------------------------------------------------------------------Main Redirection Functions------------------------------------------------------------

// To execute both input and output redirection
int executeRedirection(char* command) {
    if (checkOutputRedirection(command) != -1 || checkInputRedirection(command) != -1) {
        return 0;
    }
    return -1;
}