#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include "get_path.h"
#include "which.h"

/*
loop through pathlist until finding command and return it.  Return
NULL when not found.
arguments: takes the pathlist and command as arguments
returns the path to the command found on the computer
*/
char *which(char *command, struct pathelement *pathlist )
{
  //  we want to get the whole command
  int ret;
  //  temp variable current
  struct pathelement *curr=pathlist;
  //  pathString variable to return
  char *pathString;
  //  loop through our list of paths
  while(curr->next!=NULL) {
    pathString = malloc(strlen(curr->element)+strlen(command)+2);
    /*put together path string*/
    strcat(pathString, curr->element);
    strcat(pathString, "/");
    strcat(pathString, command);
    /*checks if pathString is accessible*/
    if(access(pathString,F_OK)!=-1) {
      return pathString;
    }
    curr=curr->next;
  }

  //  check the last one
  pathString = malloc(strlen(curr->element)+strlen(command)+2);
  strcat(pathString, curr->element);
  strcat(pathString, "/");
  strcat(pathString, command);
  if(access(pathString,F_OK)!=-1) {
    return pathString;
  }
  return "Command not found";

} /* which() */
