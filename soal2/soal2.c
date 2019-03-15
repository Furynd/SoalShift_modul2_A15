#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/wait.h> 
#include <glob.h>
#include <string.h>

int isFileExistsAccess(const char *path)
{
    // Check for file existence
    if (access(path, F_OK) == -1)
        return 0;

    return 1;
}

int main() {
  pid_t pid, sid;
  pid_t child_id;
  int status;
  int fd[2], fd2[2];

  chdir("./hatiku");
  pipe(fd);

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

  // if ((chdir("/")) < 0) {
  //   exit(EXIT_FAILURE);
  // }

  // close(STDIN_FILENO);
  // close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    child_id = fork();
    if(child_id != 0){
        while((wait(&status)) > 0);
        pipe(fd2);
        
        child_id = fork();

        if(child_id != 0){
            while((wait(&status)) > 0);
            child_id = fork();
            if(child_id != 0){
                
            }
            else {
                //child 3
                close(fd[0]);
                close(fd[1]);
                close(fd2[1]);
                int ind = 0;
                char  test[100];
                while(read(fd2[0],&test[ind++],1));
                close(fd2[0]);
                if(strcmp(test,"www-data\n")==0){
                    char *argv[3] = {"rm","elen.ku", NULL};
                    execv("/bin/rm", argv);
                }
            }
            
        } else {
            //child 2
            close(fd[1]);
            close(fd2[0]);
            dup2(fd2[1],1);
            close(fd2[1]);
            int ind = 0;
            char  test[100];
            while(read(fd[0],&test[ind++],1));
            close(fd[0]);
            if(strcmp(test,"www-data\n")==0){
                glob_t globbuf;
                globbuf.gl_offs = 3;
                glob("elen.ku", GLOB_DOOFFS, NULL, &globbuf);
                globbuf.gl_pathv[0] = "stat";
                globbuf.gl_pathv[1] = "-c";
                globbuf.gl_pathv[2] = "%U";
                execvp("stat", &globbuf.gl_pathv[0]);
            } else return 0;
            // else printf("skipped!\n");
        }
    } else {
        //child 1
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);
        if(isFileExistsAccess("./elen.ku")){

            // char *argv[5] = {"stat","-c","%G","elen.ku", NULL};
            // execv("/bin/stat", argv);
            glob_t globbuf;
            globbuf.gl_offs = 3;
            glob("elen.ku", GLOB_DOOFFS, NULL, &globbuf);
            globbuf.gl_pathv[0] = "stat";
            globbuf.gl_pathv[1] = "-c";
            globbuf.gl_pathv[2] = "%G";
            execvp("stat", &globbuf.gl_pathv[0]);
        }
        else return 0;
        // else printf("skipped!\n");
    }
    return 0;
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
}