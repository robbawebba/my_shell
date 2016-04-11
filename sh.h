#pragma once

int pid;

/*the main shell*/
int sh( int argc, char **argv, char **envp);

/*exits the shell*/
void exit_program();

/*gets and prints pid*/
void print_pid();

/*prints env of argument given*/
void printenv_arg(char *arg);

/*sets an empty environment variable*/
void printenv_whole();

/*gets the prompt from the user*/
char *get_prompt();

/*parameter length checks*/
#define PROMPTMAX 32
#define MAXARGS 10
#define MAX_PATH 4096
