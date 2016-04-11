#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#pragma once

/*node in history linked list*/
struct command_node {
    char *command; /*command name */
    struct command_node *next; /*pointer to next node in command list*/
};

/*function prototype to add node to our list
takes the command and creates a new node that gets added to the front of the list*/
struct command_node* add_command(struct command_node *head, char *command);
struct command_node *create_builtinlist();

/*function to delete a node from the list*/
struct command_node* remove_node(struct command_node *head, char *username);

/*prints out a list from beginning to end*/
void print_clist(struct command_node *head);
