#include <stdio.h>
#include <unistd.h>

int main() {
    chdir("/home/rohit");
    execlp("ls", "ls", NULL);
    return 0;
}