// To execute input and output redirection
// Return Values
// -1 : redirection does not exist
// 0 : command contains redirection and executed successfully
int executeRedirection(char* command);


// Execute cd command
// Return Values
// 0: if command executed successfully
// -1: if command is not cd 
// -2: more than one arguement to cd
// -3: error occured while executing cd
int executeCd(char* command);