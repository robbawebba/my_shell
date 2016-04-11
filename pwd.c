#include <stdio.h>
#include <unistd.h>
#include "pwd.h"

/*
pwd: print the current working directory
takes no arguments
returns the directory we are currently in*/
char *get_pwd() {
  char cwd[MAX_PATH];
  if (getcwd(cwd,sizeof(cwd))!=NULL) {
    return cwd;
  }
  else {
    perror("pwd");
  }
  return NULL;
}
