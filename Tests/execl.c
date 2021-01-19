#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	printf("Rohit\n");
	fork();
	execl("/bin/ls", "/bin/ls", NULL);
	printf("Dada\n");
}
