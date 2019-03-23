#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
        time_t t;
        time(&t);
        struct tm *waktu = localtime(&t);
        char folder[100];
        char buff[BUFSIZ];
        sprintf(folder, "/home/trash/log/%02d:%02d:%04d-%02d:%02d", waktu->tm_mday, waktu->tm_mon+1, 1900+waktu->tm_year, waktu->tm_hour, waktu->tm_min);
        mkdir(folder, 0777);
	int i;
        for(i=1; i<30; i++){
                FILE *log, *dest;
                log = fopen("/var/log/syslog", "r");
                char file[100];
                sprintf(file, "%s/log%d.log", folder, i);
                dest = fopen(file, "w");
                char cr;
                while(fread(buff, 1, BUFSIZ, log))
                {
                    fwrite(buff, 1, BUFSIZ, dest);
                }
                fclose(log);
                fclose(dest);
                sleep(60);
        }
 }
  exit(EXIT_SUCCESS);
}