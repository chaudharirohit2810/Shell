#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(int args, char* argv[]) {

    int fd = open(argv[1], O_WRONLY);
    if(fd == -1) {
        return 1;
    }   
    int bytes = write(fd, argv[2], 100);
    if(bytes != 100) {
        return 2;
    }
   

    return 0;
}