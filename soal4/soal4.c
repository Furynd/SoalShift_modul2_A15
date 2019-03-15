#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <string.h>

int main() {
  pid_t pid, sid;
  pid = fork();

  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) exit(EXIT_SUCCESS);

  umask(0);
  sid = setsid();
  if (sid < 0) exit(EXIT_FAILURE);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
 	
  int counter;
  counter=1;
  while(1) {

	struct stat cek;
	stat("Documents/makanan/makan_enak.txt", &cek);
	struct tm file = *localtime(&cek.st_atime);
	time_t now = time(NULL);
	struct tm current = *localtime(&now);
	
	double dif = difftime( mktime(&current), mktime(&file)); 

	if(dif <= 30)
	{
		char target[150];
		memset(target,'\0',sizeof(target));

		sprintf(target,"Documents/makanan/makan_sehat%d.txt",counter);

		  pid_t child_id;

		  child_id = fork();
		  
		  if (child_id < 0) exit(EXIT_FAILURE);
		  if (child_id == 0) 
		  {
		    char *argv[3] = {"touch", "file", NULL};
		    argv[1]=target;
		    execv("/usr/bin/touch", argv);
		  }
		  counter++;
	}
	sleep(5);
  }

  exit(EXIT_SUCCESS);
}
