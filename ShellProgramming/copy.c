#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int args, char* argv[]) {
    int fd_read = open(argv[1], O_RDONLY);
    if(fd_read == -1) {
        return 1;
    }

    int fd_write = open(argv[2], O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IROTH);
    if(fd_write == -1) {
        return 2;
    }

    char text[10];
    int bytes = 0;
    while (bytes = read(fd_read, text, 10))
    {
        write(fd_write, text, bytes);
    }

    return 0;
}