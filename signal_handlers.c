#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "signal_handlers.h"

// kill child processes, ctrl c handler
void sig_intHandler(int sig) {
  signal(SIGINT,sig_intHandler);
  printf("\n Cannot be terminated using Ctrl+C %d \n", waitpid(getpid(),NULL,0));
  fflush(stdout);
  return;
}

/*ctrl z handler*/
void sig_stpHandler(int sig) {
  signal(SIGTSTP, sig_stpHandler);
  printf("\n Cannot be terminated using Ctrl+Z \n");
  fflush(stdout);
}
void sig_chldHandler(int sig) {
  int saved_errno = errno;
  while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
  errno = saved_errno;
}
