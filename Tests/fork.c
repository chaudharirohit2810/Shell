#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	printf("Rohit\n");
	// It returns the id of process created
	int processID = fork();
	if(processID == 0) {
		// Parent Process
		printf("Parent\n");
	}else {
		// Child Process
		printf("Child\n");
	}
}
