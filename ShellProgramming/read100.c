#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(int args, char* argv[]) {

    int fd = open(argv[1], O_WRONLY);
    if(fd == -1) {
        return 1;
    }
    char* text = "Lorem ipsum dolor sit amet, consectetur adipisicing elit. Eveniet, sint beatae est libero sunt ullam perspiciatis distinctio voluptate magni non harum maxime ure recusandae nisi quas quisquam cum ut? Laborum placeat sequi animi rerum vel, tempore et";

    int bytes = write(fd, text, 100);
    if(bytes != 100) {
        return 2;
    }

    return 0;
}