?#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <sys/mman.h>

int main() {
  pid_t pid, sid;
  static int *a;
  a = mmap(NULL, sizeof *a, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *a=0;

  pid = fork();
  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) exit(EXIT_SUCCESS);

  umask(0);
  sid = setsid();
  if (sid < 0) exit(EXIT_FAILURE);
  
  close(STDERR_FILENO);
  close(STDIN_FILENO);
  close(STDOUT_FILENO);

  while(1) {

	char folder[100];
	char fil[100];
 	*a=*a%30;
        if(*a==0)
	{
	     time_t t = time(NULL);
	     struct tm tm = *localtime(&t);
	     memset(folder,'\0',sizeof(folder));
	     sprintf(folder, "%d:%d:%d-%d:%d", tm.tm_mday, tm.tm_mon + 1, 
		     tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
	}	

        pid_t child_id;
        int status;

	child_id = fork();

        if (child_id == 0 && (*a)==0) 
        {
		char *argv[4] = {"mkdir", "-p", "Folder", NULL};
		argv[2]=folder;
		execv("/bin/mkdir", argv);
        } 
        else 
        {
		while ((wait(&status)) > 0);
		memset(fil,'\0',sizeof(fil));
		*a=*a+1;
		pid_t child_idd;
       		int status1;
		child_idd = fork();
		if (child_idd == 0) 
		{
			sprintf(fil, "%s/log%d.log", folder, *a);
			char *argv[3] = {"touch", "file", NULL};
			argv[1]=fil;
			execv("/usr/bin/touch", argv);
		} 
		else 
		{
			while ((wait(&status1)) > 0);
		        char ch, source_file[100], target_file[100];
		        FILE *source, *target;

		        memset(source_file,'\0',sizeof(source_file));
		        strcpy(source_file,"/var/log/syslog");
		        source = fopen(source_file, "r");
		     
		        if (source == NULL){exit(EXIT_FAILURE);}

			sprintf(fil, "%s/log%d.log", folder, *a);
			memset(target_file,'\0',sizeof(target_file));
			strcpy(target_file,fil);

			target = fopen(target_file, "w");
		     
			if (target == NULL)
			{
			  fclose(source);
			  exit(EXIT_FAILURE);
			}

		        while ((ch = fgetc(source)) != EOF) fputc(ch, target);
			fclose(source);
		        fclose(target);
		}  
        }

    sleep(60);
  }
  
  exit(EXIT_SUCCESS);
}
