#pragma once

/*control z handler*/
void sig_stpHandler(int sig);
/*control c handler*/
void sig_intHandler(int sig); //must deal with other processes
/* sigchld handler */
void sig_chldHandler(int sig);
