#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
int main() {
	int pid;
	char cmd[128];
	while(1) {
			printf("prompt>");
			scanf("%s", cmd);
			pid = fork();
			if(pid == 0) {
				//printf("in child %d \n", pid);
				if(execl(cmd, cmd, NULL) == -1){
					exit(0);
				}
			} else {
				//printf("in parent %d \n", pid);
				wait(0);
			}
	}
	return 0;
}
