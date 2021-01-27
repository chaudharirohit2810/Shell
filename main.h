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

// SIGINT (Ctrl + C) handler for parent
void startCtrlCHandler();

// SIGTSTP(Ctrl + Z) handler for parent
void startCtrlZHandler();


//------------------------------------------------------------------Commands related to signals----------------------------------------------------------------

// To execute "fg" command
void executeFg(int index);

// To execute "bg" command
void executeBg(int index);

// To execute "jobs" command
void executeJobs();


//---------------------------------------------------------------------Utils for signal handlers----------------------------------------------------------------
// Setter for currentpid
void setCurrentpid(int pid);

// Setter for current command
void setCurrentCommand(char* cmd);

// To add process running in background
void addStoppedProcess(int pid, char* cmd);

