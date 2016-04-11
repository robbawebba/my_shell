#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utmpx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_list.h"
#include "watchuser.h"

int watchuser_switch = 0;
struct command_node *userlist = NULL;

void create_watchthread() {
  pthread_t watchuser_pid;
  pthread_create(&watchuser_pid, NULL, watchuser_thread, "watchuser thread");
  watchuser_switch = 1;
}

/*checks all of the users logged on*/
static void *watchuser_thread(void *param) {
  while(1) {
    userlist_check(userlist);
    sleep(30);
  }
}

void userlist_check(struct command_node *head) {
  struct command_node *curr = head;
  while(curr!=NULL) { //each user in userlist
    // printf("user yo: %s", curr->command);
    checkuser_loggedin(curr->command);
    curr=curr->next;
  }
}

int checkuser_loggedin(char *username)
{
  struct utmpx *up;

  setutxent();			/* start at beginning */
  while (up = getutxent() )	/* get an entry */
  {
    if ( up->ut_type == USER_PROCESS )	/* only care about users */
    {
      if(strcmp(up->ut_user, username)==0) { /*check if user being watched matches*/
          printf("%s has logged on %s from %s \n", up->ut_user, up->ut_line, up ->ut_host);
      }
    }
  }
  return 0;
} /* main() */
