#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main() {
	pid_t pid, sid;
	char pwd[] = "/home/arifdarma/modul2/gambar/";

	pid = fork();

	if (pid < 0) {
   		exit(EXIT_FAILURE);
  	}
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();

	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir(pwd)) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
  	close(STDOUT_FILENO);
  	close(STDERR_FILENO);

  	while(1) {
		DIR* FD;
		struct dirent* in_file;
		char lama[100]={0};
		char baru[100]={0};
		char tujuan[100]={0};
		FD=opendir(".");
		while(in_file=readdir(FD)){
			strcpy(lama,in_file->d_name);
			if(strcmp("_grey.png", &lama[strlen(lama)-9])!=0 && strcmp(".png",&lama[strlen(lama)-4])==0){
				memset(baru,0,sizeof baru);
				strncpy(baru,lama,strlen(lama)-4);
				strcat(baru,"_grey.png");
				strcpy(tujuan,pwd);
				strcat(tujuan,baru);
				rename(lama,tujuan);
			}
		}
		closedir(FD);
    	sleep(10);
  	}
  	exit(EXIT_SUCCESS);
}
