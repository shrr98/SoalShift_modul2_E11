#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int main() {

    pid_t daemon, sid;

    time_t t = time(0);
    int menitAwal = localtime(&t)->tm_min;
    int cattogrep[2], greptowrite[2];

    
    if(pipe(cattogrep)<0)
        exit(EXIT_FAILURE);
    if(pipe(greptowrite)<0)
        exit(EXIT_FAILURE);
	daemon = fork();

	if (daemon < 0) {
		exit(EXIT_FAILURE);
	}

	if (daemon > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();

	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir(".")) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

    while(1){
    pid_t master = fork();

    if(master==0) sleep(5);
    else{
    int child_id = fork();

        pid_t ls_child = fork();
        if(ls_child==0){ // adiknya anak
            close(greptowrite[0]);
            close(cattogrep[0]);
            close(greptowrite[1]);
            dup2(cattogrep[1], STDOUT_FILENO);
            close(cattogrep[1]);
            close(cattogrep[0]);
            char* argv[] = {"cat", "/var/log/syslog", NULL};
            execv("/bin/cat", argv);                   
        }
        else{
            pid_t grep_child=fork();
            
            if(grep_child==0){ //child
                time_t tim = time(NULL);
                struct tm *now = localtime(&tim);
                char time[6];
                sprintf(time, "%02d:%02d", now->tm_hour, now->tm_min);
                
                close(cattogrep[1]);
                close(greptowrite[0]);
               
                dup2(cattogrep[0], STDIN_FILENO);
                close(cattogrep[0]);
                close(cattogrep[1]);
                dup2(greptowrite[1], STDOUT_FILENO);
                close(greptowrite[1]);
                close(greptowrite[0]);

                char* argv[] = {"grep", time, NULL};
                execv("/bin/grep", argv);     
            }
            else{ //parent
                time_t tim1 = time(NULL);
                struct tm *now1 = localtime(&tim1);
                int currMin = now1->tm_min;
                pid_t writing = fork();
                if(writing==0){
                    if((currMin%30) == menitAwal){
                    char newDir[100];
                    sprintf(newDir, "/home/trash/log/%02d:%02d:%04d-%02d:%02d",
                            now1->tm_mday, now1->tm_mon, now1->tm_year, now1->tm_hour, now1->tm_min);
                    char *argv[] = {"mkdir","-p", newDir, NULL};
                    execv("/bin/mkdir", argv);
                    }
                    else exit(EXIT_SUCCESS);
                }
                else{
                    waitpid(writing, NULL, WNOHANG);
                    char buff[4096];                  
                    char filename[200];
                    int count = currMin;
                    if(currMin<30) currMin = menitAwal;
                    else currMin = 30+menitAwal;
                    count = count-currMin+1;
                    sprintf(filename, "/home/trash/log/%02d:%02d:%04d-%02d:%02d/log%d.log", 
                            now1->tm_mday, now1->tm_mon, now1->tm_year, now1->tm_hour, currMin, 
                            count);
                    
                    close(cattogrep[1]);
                    close(cattogrep[0]);
                    close(greptowrite[1]);
                    FILE* file = fopen(filename, "w");
                    while(read(greptowrite[0], buff, sizeof(buff))){
                        
                    fprintf(file, "%s", buff);

                    }

                    close(greptowrite[0]);
                
                    fclose(file);
                    exit(EXIT_SUCCESS);
                }
        }
    }
    }
}
}