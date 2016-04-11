#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kill.h"

//kills the specified process
void sh_kill(char **args, int argsct) {
  if(argsct == 2) {
    kill(atoi(args[1]), SIGKILL);
  }
  //to fix
  else if(strstr(args[1], "-")!=NULL) {
    printf("%s\n", args[1]);
    kill(atoi(args[2]), atoi(++args[1]));
  }
}
