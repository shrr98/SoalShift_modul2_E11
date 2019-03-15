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

int main() {
  char directory[] = "/home/trash/Documents/makanan/";
  char file_enak[] = "/home/trash/Documents/makanan/makan_enak.txt";

  pid_t daemon, sid;

    int link[2];
    if (pipe(link)==-1)
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
    pid_t sleep_id = fork();
    if(sleep_id<0) exit(EXIT_FAILURE);
    else if(sleep_id>0){
      pid_t pid = fork();
      if (pid<0)
        exit(EXIT_FAILURE);
      else if(pid==0){  // child
        close(link[0]);
        dup2(link[1], STDOUT_FILENO);
        close(link[1]);
        close(link[0]);
        char *argv[] = {"find", file_enak, "-amin", "-0.5", NULL};
        execv("/usr/bin/find", argv);
      }
      else {
        DIR *dir;
        struct dirent *file;
        close(link[1]);
        char buffer[128]="\0", new_file[100];
        int count;
        int n_buff = read(link[0],buffer, sizeof(buffer));
        
        if(n_buff>0){   // kalau pernah dibuka
          dir = opendir(directory);
          count = 0;
          while(file = readdir(dir)){
            if(strstr(file->d_name, "makan_sehat")!=NULL){
              count++;
            }
          }
          close(link[0]);
          close(link[1]);
          sprintf(new_file, "%smakan_sehat%d.txt", directory, ++count);
          char *argv[] = {"touch", new_file, NULL};
          execv("/usr/bin/touch", argv);
        }
      }
    }
    else{
      sleep(5);
    }
  }
 
  return 0;
}