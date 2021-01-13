#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(int args, char* argv[]) {
    int fd = open(argv[1], O_RDWR);
    char text[10];

    if(fd == -1) {
        return 1;
    }
    
    lseek(fd, 4, SEEK_SET);
    int bytes = read(fd, text, 6); // read 5th to 10th bytes
    text[bytes] = '\0';
    
    if(bytes != 6) {
        return 3;
    }

    printf("%s", text);
    
    return 0;
}