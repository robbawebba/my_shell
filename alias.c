#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"

struct alias_node *head;

struct alias_node* add_alias(struct alias_node *head, char *name, char *cmnd) {
    struct alias_node *curr = head;

    while(curr != NULL)
    {
        if(strcmp(name, curr->alias) == 0) {
            strcpy(curr->command, cmnd);
            return head;
        }
        curr=curr->next;
    }

    struct alias_node *tmp = malloc(sizeof(struct alias_node));
    curr = head;

    tmp->alias = malloc(sizeof(name)+1);
    tmp->command = malloc(sizeof(cmnd)+1);
    strcpy(tmp->command, cmnd);
    strcpy(tmp->alias, name);
    tmp->next=NULL;
    //if there are no elements in the list
    if (curr==NULL) {
        head=tmp;
    }
    //if its one element in list
    else if(curr->next==NULL) {
        curr->next=tmp;
    }
    else {
    while(curr->next!=NULL) {
        curr=curr->next;
    }
    curr->next=tmp;
  }
  return head;
}

void print_list(struct alias_node *head)
{
    struct alias_node *curr = head;
    while(curr != NULL)
    {
        printf("%s      %s\n", curr->alias, curr->command);
        curr=curr->next;
    }
}

void free_list(struct alias_node *head)
{
    while (head != NULL)
    {
        struct alias_node *next = head->next;
        free (head->alias);
        free (head->command);
        head = next;
    }
}
