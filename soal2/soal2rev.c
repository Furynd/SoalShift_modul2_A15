#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int isFileExistsAccess(const char *path)
{
    // Check for file existence
    if (access(path, F_OK) == -1)
        return 0;

    return 1;
}

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

  if ((chdir("./hatiku")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    struct stat sb;
    if(isFileExistsAccess("./elen.ku")){
      if(stat("./elen.ku",&sb)!= -1) {
          printf("%ld %ld\n",(long) sb.st_gid, (long)sb.st_uid);
      }
      struct passwd *pw = getpwuid(sb.st_uid);
      struct group  *gr = getgrgid(sb.st_gid);
      printf("%s %s\n", pw->pw_name, gr->gr_name);
      if(strcmp("www-data",pw->pw_name)==0){
        chmod("./elen.ku",777);
        remove("./elen.ku");
      }
    }
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
}