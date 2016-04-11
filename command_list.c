#include "command_list.h"

struct command_node *head;

/* pushes a new node to the front of the list */
struct command_node* add_command(struct command_node *head, char *cmnd) {
  struct command_node *curr = head;
  struct command_node *tmp = malloc(sizeof(struct command_node));

  tmp->command=malloc(strlen(cmnd)+1);
  strcpy(tmp->command, cmnd);
  tmp->next=NULL;

  /* if no elements in the list */
  if (curr==NULL) {
    head=tmp;
  }
  /* if there are already elements in the list */
  else {
    /* push the node onto the front of the list */
    tmp->next = curr;
    head = tmp;
  }
  return head;
}

/*creates a list of built-in commands*/
struct command_node *create_builtinlist() {
  struct command_node *builtin = NULL;
  builtin = add_command(builtin, "which");
  builtin = add_command(builtin, "where");
  builtin = add_command(builtin, "pwd");
  builtin = add_command(builtin, "cd");
  builtin = add_command(builtin, "pid");
  builtin = add_command(builtin, "kill");
  builtin = add_command(builtin, "history");
  builtin = add_command(builtin, "printenv");
  builtin = add_command(builtin, "exit");
  builtin = add_command(builtin, "alias");
  builtin = add_command(builtin, "prompt");
  return builtin;
}

struct command_node *remove_node(struct command_node *head, char *username) {
  int ret;
  struct command_node *curr = head;
  struct command_node *d_node = curr; //keep track of node to delete

  if (head == NULL)
    return head;

  /*check for first node*/
  if(strcmp(curr->command, username) == 0) {
    head = head->next;
    // free(d_node->command);
    // free(d_node);
    return head;
  }

  /*iterate through list*/
  while (curr->next != NULL) {
    d_node = curr->next;
    if(d_node == NULL) {
      return head;
    }
    if(strcmp(d_node->command,username)==0){
      if(d_node->next != NULL) {
        curr->next = d_node->next;
        d_node->next = NULL;
      }
      else {
        curr->next = NULL;
      }
      // free(d_node->command);
      // free(d_node);
      return head;
    }
    curr=curr->next;
  }
  fprintf(stderr, "%s is not in the watchusers list\n", username);
  return head;
}

void print_clist(struct command_node *head) {
  struct command_node *curr = head;
  int i=1;
  while (curr != NULL) {
    printf("user: %s - %d\n", curr->command, i);
    i++;
    curr=curr->next;
  }
}
