#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
int main() {
	int pid, pfd[2];
	char cmd[128], buf[16];
	while(1) {
			printf("prompt>");
			scanf("%s", cmd);
			pipe(pfd);
			pid = fork();
			if(pid == 0) {
				close(pfd[0]);
				write(pfd[1], "hello", 6);
				execl(cmd, cmd, NULL);
			} else {
				close(pfd[1]);
				read(pfd[0], buf, 6);
				printf("parent read: %s\n", buf);
				wait(0);
			}
	}
	return 0;
}
