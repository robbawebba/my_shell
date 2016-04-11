#pragma once

struct alias_node {
    char *alias; /*alias name */
    char *command; /* alias command */
    struct alias_node *next; /*pointer to next node in command list*/
};

/*function prototype to add node to our list
takes the command and creates a new node that gets added to the end of the list*/
struct alias_node* add_alias(struct alias_node *head, char *name, char *command);
/* function prototype to print all the nodes in the list */
void print_list(struct alias_node *head);
/* function prototype to free memory of all nodes in the list */
void free_list(struct alias_node *head);
/* function prototype to check list aliases for the given command */
int check_aliases(struct alias_node *head, char *command);
