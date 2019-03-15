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

    
    if(pipe(cattogrep)<0)
        exit(EXIT_FAILURE);
    if(pipe(greptowrite)<0)
        exit(EXIT_FAILURE);	}

	if ((chdir(".")) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

    while(1){
        pid_t master=fork();
        if(master==0) sleep(10);
        else{
            char newfile[100];
            time_t tim = time(0);
            int count;
            struct tm *now = localtime(&tim);
            if(now->tm_min >= menitAwal) count = 1+ now->tm_min - menitAwal;
            else count = 31 - (menitAwal - now->tm_min);
            if(count>30) count-=30;
            sprintf(newfile, "/home/trash/log/log%d.log", count);
            pid_t copying = fork();
            if(copying==0){
                char *argv[] = {"cp", "/var/log/syslog", newfile, NULL};
                execv("/bin/cp", argv);
            }
            else{
                wait(NULL);
                pid_t mkdir=fork();
                if(mkdir==0){
                    int currMin = now->tm_min;
                    if((currMin%30) == menitAwal){
                    char newDir[100];
                    sprintf(newDir, "/home/trash/log/%02d:%02d:%04d-%02d:%02d",
                            now->tm_mday, now->tm_mon, now->tm_year, now->tm_hour, now->tm_min);
                    char *argv[] = {"mkdir","-p", newDir, NULL};
                    execv("/bin/mkdir", argv);
                    }
                    else exit(EXIT_SUCCESS);
                }
                else{
                    wait(NULL);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }
}