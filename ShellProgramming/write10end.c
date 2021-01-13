#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int args, char* argv[]) {
    int fd = open(argv[1], O_RDWR);
    if(fd == -1) {
        return 1;
    }
    char text[11];
    int bytes = 1;
    while(bytes = read(fd, text, 10))
    {
        if(bytes == -1) {
            return 2;
        }
        text[bytes] = '\0';
        printf("%s", text);
    }
    
    char* text2 = "My name is Rohit";
    int res = write(fd, text2, 10);
    if(res != 10) {
        return 3;
    } 
    return 0;
}