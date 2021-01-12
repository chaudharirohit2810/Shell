#include<stdio.h>
#include<unistd.h>

int main(int args, char* argv[]) {

    int val = link(argv[1], argv[2]);

    if (val == -1)
    {
        return 1;
    }

    val = unlink(argv[1]);
    if (val == -1)
    {
        return 2;
    }
    
    return 0;
}