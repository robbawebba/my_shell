#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include "command_list.h"
#include "history.h"

/*
pass in arguments for display, size of list, and head of list
*/
void history(int displayct, int listct, struct command_node *head) {
  struct command_node *curr = head;
  int i=1;
  //if given an argument, print out until that number or until end of list
  if(displayct > 0) {
    while(i<=displayct && curr!=NULL){
      printf("%d  :  %s\n", listct--, curr->command);
      curr=curr->next;
      i++;
    }
  }
  //if no args, default print history list is 10
  else if (displayct==-1){
    while(i < 11 && curr != NULL) {
      printf("%d  :  %s\n", listct--, curr->command);
      curr=curr->next;
      i++;
    }
  }
}
