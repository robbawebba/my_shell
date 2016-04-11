#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse_commandline.h"

/*
This function parses the input passed from above
paramter: commandline input
returns the parsed line of arguments to pass into the other methods
*/
int parseLine(char *commandline, char** args) {
  char *input = calloc(strlen(commandline) + 1, sizeof(char));
  char *split;
  int argsct=0;
  input = commandline;
  // tokenize our string
  split = strtok(input, " ");
  while(split!=NULL && argsct <= MAXARGS) {
    args[argsct] = split;
    argsct++;
    split = strtok(NULL, " ");
  }
  return argsct;
}
