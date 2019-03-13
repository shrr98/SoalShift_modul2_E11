#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
	pid_t pid, sid;

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

	if ((chdir(".")) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	while(1){
		pid_t child_id = fork();
		if(child_id==0){
			pid_t grandchild_id = fork();
			if(grandchild_id<0) exit(EXIT_FAILURE);
			else if(grandchild_id==0){
				sleep(10);
				struct stat buf;
				if(stat("hatiku/elen.ku", &buf)==-1)
					exit(EXIT_FAILURE);
				struct group *gr = getgrgid(buf.st_gid);
				struct  passwd *pw = getpwuid(buf.st_uid);
				if(strcmp(pw->pw_name, "www-data")==0 && strcmp(gr->gr_name, "www-data")==0){
					char *argv[4] = {"chmod", "777", "hatiku/elen.ku", NULL};
					execvp("chmod", argv);
				}
			}
			else{
				int status;
				while((wait(&status))>0);
				char *argv[] = {"rm", "hatiku/elen.ku", NULL};
				execv("/bin/rm", argv);
			}
		}
		else {
			sleep(3);
		}
	}
}
