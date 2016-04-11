#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "cd.h"

int cd(char *directory){
  //apppend our directory to currentdir
  char pathString[1024];
  //get the current directory to append our args to
  getcwd(pathString,sizeof(pathString));
  //put together the string to check
  strcat(pathString, "/");
  if (strcmp(directory,"-")==0) {
    strcat(pathString, "..");
    return chdir(pathString);
  }

  //IF THE DIRECTORY IS PASSED IN WHOLE PATH FORM
  if(strstr(directory,"/")) {
    if(chdir(directory)!=0)
    return chdir(directory);
  }
  else {
    //IF NOT PASSED IN WHOLE
    strcat(pathString, directory);
    //if it doesn't exist
    int help = chdir(pathString);
    if(help!= 0) {
      perror("cd");
    }
    return help;

  }
}
