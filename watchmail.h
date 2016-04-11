#pragma once

/* struct definition for the node of an inbox to watch */
struct inbox_node {
    pthread_t tid;
    char *file_name; /* name of file to watch */
    struct inbox_node *next; /*pointer to next node in inbox list*/
};

/* function prototype for adding a new watchmail thread */
struct inbox_node* add_watchmail(struct inbox_node* head, char *filename);

/* function prototype for closing a watchmail thread */
struct inbox_node* remove_watchmail(struct inbox_node* head, char *filename);

/*function prototype for checking of a file has changed */
static void *check_file(void *param);

/* function prototype to print the list of currently watched inboxes */
void print_inboxes(struct inbox_node *head);
