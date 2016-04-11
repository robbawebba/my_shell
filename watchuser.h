#pragma once
#include "command_list.h"

/*our global userlist*/
extern struct command_node *userlist;

/*whether or not thread has started*/
extern int watchuser_switch;

/*creates the new thread for watchuser
switches the watchuser_switch value a thread is created only once*/
void create_watchthread();

/*our thread function to watch users
gets the current users on the server
and compares to the watchuser list we are keeping track of*/
static void *watchuser_thread(void *param);

/*goes through global userlist and checks if any are logged on*/
void userlist_check(struct command_node *head);

/*checks if given username is logged on
the system*/
int checkuser_loggedin(char *username);
