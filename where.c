#include "get_path.h"
#include "command_list.h"
#include "alias.h"
#include "where.h"

void where(char *command, struct alias_node *aliases, struct command_node *head, struct pathelement *pathlist )
{
  /* similar to which, loop through finding all locations of command */
  /* Check list of Aliases by looping through it */
  struct alias_node *current_alias = aliases;

  while(current_alias != NULL) {
    /* if alias matches the given command */
    if(strcmp(current_alias->alias, command)==0) {
      /* print out alias and full command */
      printf("%s is aliased to %s\n", command, current_alias->command);
      break; /* found aliased command, exit alias loop */
    }
    current_alias = current_alias->next; /* move to next alias */
  }

  /* Check Built-In Functions */
  struct command_node *current = head;

  while(current != NULL) {
    /* if command is a built-in function */
    if(strcmp(current->command, command)==0) {
      /* print out built-in command */
      printf("%s is a shell built-in\n", command);
      break; /* found built-in command, exit built-in loop */
    }
    current = current->next; /* move to next built-in command */
  }

  /* Check Path */
  struct pathelement *curr=pathlist;
  char *pathString;
  //  loop through our list of paths
  while(curr->next!=NULL) {
    pathString = malloc(strlen(curr->element)+strlen(command)+2);
    strcat(pathString, curr->element);
    strcat(pathString, "/");
    strcat(pathString, command); /* append command to current path */
    //checks if path exists and prints
    if(access(pathString,F_OK)!=-1) {
      printf("%s\n", pathString);
    }
    //loops to the next
    curr=curr->next;
  }
} /* where() */
