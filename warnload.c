#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "warnload.h"

#ifdef HAVE_KSTAT
#include <kstat.h>
#endif

/*global variable for warn load level*/
double warn_lvl = 0.0;

/*Modified to only get 1min load levels*/
int get_load(double *loads)
{
#ifdef HAVE_KSTAT
  kstat_ctl_t *kc;
  kstat_t *ksp;
  kstat_named_t *kn;

  kc = kstat_open();
  if (kc == 0)
  {
    perror("kstat_open");
    exit(1);
  }

  ksp = kstat_lookup(kc, "unix", 0, "system_misc");
  if (ksp == 0)
  {
    perror("kstat_lookup");
    exit(1);
  }
  if (kstat_read(kc, ksp,0) == -1)
  {
    perror("kstat_read");
    exit(1);
  }

  kn = kstat_data_lookup(ksp, "avenrun_1min");
  if (kn == 0)
  {
    fprintf(stderr,"not found\n");
    exit(1);
  }
  loads[0] = kn->value.ul/(FSCALE/100);

  kstat_close(kc);
  return 0;
#else
  /* yes, this isn't right */
  loads[0] = 0;
  return -1;
#endif
} /* get_load() */

//we should be passing in the get_loads as an argument that we can then reference
static void *warn_thread(void *param){
  double loads[1]; //store the load level here
  int i = 0;
  const char *name=param;
  int ret=-1;
  get_load(loads); //get_loads here, should be able to access now
  // printf("%.2lf warn load. %.2lf CURRENT LVLS \n", warn_lvl, loads[0]/100);
  while (1) {
    if (warn_lvl == 0.0) {
      // perror("warnload");
      pthread_exit(&ret);
    }
    if(loads[0]/100 > warn_lvl) { //checks the warn level against the current load
      printf("Warning: Load level is %.2lf\n", loads[0]/100);
    }
    sleep(30);
  }
}

void create_warnthread() {
  pthread_t warn_pid;
  pthread_create(&warn_pid, NULL, warn_thread, "WARNING THREAD");
}
