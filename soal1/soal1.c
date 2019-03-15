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
int b;
int main() 
{
  pid_t pid, sid;	
  int b;

  pid = fork();

  if (pid < 0) {exit(EXIT_FAILURE);}
  if (pid > 0) {exit(EXIT_SUCCESS);}

  umask(0);
  sid = setsid();
  if (sid < 0) {exit(EXIT_FAILURE);}

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

 while(1) 
 {
    char path[10];
    memset(path,'\0',sizeof(path));
    strcpy(path,"campur2");
    struct dirent *dp;
    DIR *dir = opendir(path);

    if (dir)
    {  

            while ((dp = readdir(dir)) != NULL)
            {
                if(strstr(dp->d_name,".png")) 
                {       
                        int a=strlen(dp->d_name);
                        char tmp[a+25];
                        char src[a+15];
                        memset(tmp,'\0',sizeof(tmp));
                        memset(src,'\0',sizeof(src));
                        strcpy(src,"campur2/");
                        strcpy(tmp,"modul2/gambar/");

                        strcat(src,dp->d_name);
                        strncat(tmp,dp->d_name,a-4);
			strcat(tmp,"_grey.png");
                        rename(src,tmp);
                }
            }
            closedir(dir);
    }

   sleep(5);
 }

  exit(EXIT_SUCCESS);
}
