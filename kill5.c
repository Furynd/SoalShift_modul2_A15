#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
char *lima[] = {"pkill","lima",NULL};
execvp("pkill",lima);
}
