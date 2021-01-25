// Separate arguements and command
// Return value: Number of arguements
int tokenizeValues(char* args[], char cmd[]);


// Check for "exit" command
// Return values
// 0: command is "exit", exit out of terminal
// -1: command does not contain "exit"
int checkExit(char* cmd);


// Wrapper function to execute command
// Return values
// 0: Command executed successfully
// -1: command execution failed
int executeCommand(char* command);


// Separate background processes
// Return Value: Number of arguements, -1 if command does not contain background processes
int isBackground(char* command, char* bgProcesses[] );


// Ignore Ctrl+z and Ctrl+c signal in child
void ignoreSignals();
