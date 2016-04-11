#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "watchmail.h"

/* function for adding a new watchmail thread */
struct inbox_node* add_watchmail(struct inbox_node *head, char *filename) {
  pthread_t new_tid;
  /*create a new pthread */
  int err = pthread_create(&new_tid, NULL, check_file, (void *)filename);
  if(err) { /* if pthread_create returned an error (int other than 0) */
    errno = err;
    perror("watchmail");
    return head;
  }
  /* TODO: need  to handle errors */

  struct inbox_node *curr = head;
  struct inbox_node *tmp = malloc(sizeof(struct inbox_node));

  tmp->file_name=malloc(sizeof(filename)+1);
  tmp->tid = new_tid; /* assign the new thread id to tid in new node */
  strcpy(tmp->file_name, filename);  /* copy file name to new node */
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

/* function for closing a watchmail thread */
struct inbox_node* remove_watchmail(struct inbox_node *head, char *filename) {
  int ret;
  struct inbox_node *curr = head;
  struct inbox_node *deleteNode = curr;
  /* if the given list is empty */
  if(head == NULL) {
    return head;
  }
  /* check very first node */
  if (strcmp(curr->file_name, filename) == 0) {
    ret = pthread_cancel(deleteNode->tid);
    if (ret != 0) {
      errno = ret;
      perror("watchmail");
      return head;
    }
    else {
      head = head->next;
      /* free memory of the inbox attributes */
      free(deleteNode->file_name);
      free(deleteNode); /* finally free the node pointer */
      return head;

    }
  }

  while(curr->next != NULL)
  {
    deleteNode = curr->next;
    if(deleteNode == NULL) {
      return head;
    }
    if(strcmp(deleteNode->file_name, filename) == 0) {
      int ret = pthread_cancel(deleteNode->tid);
      if (ret != 0) {
        errno = ret;
        perror("watchmail");
        return head;
      }
      /* node is somewhere in the middle of the list */
      else if (deleteNode->next != NULL) {
        curr->next = deleteNode->next;
        deleteNode->next = NULL;
      }
      else { /* node is last in a list with 2+ nodes */
        curr->next = NULL;
      }
      /* free memory of the inbox attributes */
      free(deleteNode->file_name);
      free(deleteNode); /* finally free the node pointer */
      return head;
    }
    curr=curr->next;
  }
  fprintf(stderr, "%s is not being watched\n", filename);
  return head;
}

static void *check_file(void *param) {
  int ret=-1; /*return value for pthread_exit */
  int oldsize = 0;
  int newsize = 0;
  struct stat *fileinfo; /* declare stat struct to check file */
  struct timeval tv; /* struct for finding current time */
 struct tm* ptm; /* struct for finidng current time */
 char time_string[40]; /* string representation of time */
 long milliseconds; /* time number */

  fileinfo = malloc(sizeof(struct stat));
  const char *name=param;
  if(stat(name, fileinfo)) { /* get info on file 'name' and save it to fileinfo */
    perror("watchmail");
    pthread_exit(&ret); /* exit if there's an error with stat */
  }
  oldsize = fileinfo->st_size; /* get size of file in bytes */
  while (1) {
    /* get info on file 'name' and save it to fileinfo */
    if(stat(name, fileinfo)) { /*if statement to check for errors */
      perror("watchmail");
      pthread_exit(&ret); /* exit if there's an error with stat */
    }
    newsize = fileinfo->st_size; /* get size of file in bytes */
    if (newsize > oldsize) { /* if the size increased
      /* Obtain the time of day, and convert it to tm struct. */
      gettimeofday (&tv, NULL);
      ptm = localtime (&tv.tv_sec);
      /* Format date and time */
      strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
      /* Compute milliseconds from microseconds. */
      milliseconds = tv.tv_usec / 1000;
      /* Print notification with mailbox and time */
      printf("\aYou've got mail in %s at %s\n", name, time_string);
    }

    oldsize = newsize;
    sleep(1);
  }
}
