#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();
    if(pid==0){
        char *argv[] = {"pkill", "soal5", NULL};
        execv("/usr/bin/pkill", argv);
    }
    else{
        exit(EXIT_SUCCESS);
    }
}