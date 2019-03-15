
// C program to demonstrate use of fork() and pipe()
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include <dirent.h>

int main()
{
    pid_t unzip_child = fork();

    if(unzip_child==0) {       // anak
        char *argv[] = {"unzip", "campur2.zip", NULL};
        execv("/usr/bin/unzip", argv);
    }
    else {
        wait(NULL);
        int lstogrep[2], greptowrite[2];
        if(pipe(lstogrep)==-1){
            exit(EXIT_FAILURE);
        }
        if(pipe(greptowrite)==-1){
            exit(EXIT_FAILURE);
        }
        pid_t ls_child = fork();
        if(ls_child==0){ // adiknya anak
            close(greptowrite[0]);
            close(lstogrep[0]);
            close(greptowrite[1]);
            dup2(lstogrep[1], STDOUT_FILENO);
            close(lstogrep[1]);
            close(lstogrep[0]);
            char* argv[] = {"ls", "campur2", NULL};
            execv("/bin/ls", argv);                   
        }
        else{
            pid_t grep_child=fork();
            
            if(grep_child==0){ //child
                close(lstogrep[1]);
                close(greptowrite[0]);
               
                dup2(lstogrep[0], STDIN_FILENO);
                close(lstogrep[0]);
                close(lstogrep[1]);
                dup2(greptowrite[1], STDOUT_FILENO);
                close(greptowrite[1]);
                close(greptowrite[0]);

                char* argv[] = {"grep", "txt\\>", NULL};
                execv("/bin/grep", argv);     
            }
            else{ //parent
                char buff[4096];
                close(lstogrep[1]);
                close(lstogrep[0]);
                close(greptowrite[1]);
                read(greptowrite[0], buff, sizeof(buff));

                close(greptowrite[0]);
                FILE* file = fopen("daftar.txt", "w");
                fprintf(file, "%s", buff);

                fclose(file);
                exit(EXIT_SUCCESS);
            }
        }
    }
}
