#include "list.h"

void list ( char *dir )
{
  DIR *tempdir;
  struct dirent *entry;

  tempdir = opendir(dir);
  //check here if tempdir is null
  if (tempdir == NULL) {
    errno = ENOENT;
    perror("list");
    return;
  }
  //prints out the directory
  printf("%s:\n", dir);
  while((entry=readdir(tempdir))!=NULL) {
    printf("%s\n",entry->d_name);
  }
  printf("\n");
  closedir(tempdir);
} /* list() */
