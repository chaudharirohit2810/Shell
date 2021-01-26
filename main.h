// Execute input and output redirection commands
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
int executeChangeDirectory(char* command);



// -------------------------------------------------------------------Signal Handlers---------------------------------------------------------------------
// To handle SIGINT in parent
void startCtrlCHandler();

// To handle SIGTSTP in parent
void startCtrlZHandler();

// Setter for currentpid
void setCurrentpid(int pid);

// To set current command
void setCurrentCommand(char* cmd);

// To execute "fg" command
void executeFg();

// To handle "bg" command
void executeBg();

// To add process running in background
void addStoppedProcess(int pid, char* cmd);

