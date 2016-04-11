#include <stdlib.h>
#include "setenv.h"
/*sets an empty environment variable*/
void sh_setenv(char *arg) {
  setenv(arg," ",1);
}

void sh_setenv_args(char *arg1, char *arg2) {
  setenv(arg1,arg2,1);
}
