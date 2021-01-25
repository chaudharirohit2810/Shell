#include <stdio.h>
#include <signal.h>
void signalhandler(int sig_num){
   signal(SIGTSTP, signalhandler);
   printf("Cannot execute Ctrl+Z\n");
}
int main(){
   int a = 1;
   signal(SIGTSTP, signalhandler);
   while(a){
   }
   return 0;
}
