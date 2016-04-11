#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#include "get_path.h"
#include "parse_commandline.h"
#include "command_list.h"
#include "alias.h"
#include "list.h"
#include "cd.h"
#include "which.h"
#include "where.h"
#include "watchuser.h"
#include "warnload.h"
#include "watchmail.h"
#include "history.h"
#include "kill.h"
#include "pwd.h"
#include "setenv.h"
#include "signal_handlers.h"
#include "showload.h"
#include "redirect.h"
#include "pipe.h"

int sep_commands(char** rightSide, char** leftSide, char** args, const char* commandline){
  int i = 0;
  int piped = 0;
  int j = 0;
  int k = 0;
  while(args[i] != NULL){
    // printf("args [%d] :  %s\n", i, args[i]);
    if(piped == 1){
      rightSide[j] = args[i];
      j++;
    }
    if((strcmp(args[i],"|")==0) || strcmp(args[i],"|&")==0){
      // printf("hi caught %s\n", args[i]);
      piped = 1;
    }
    else{
      leftSide[i] = args[i];
    }
    i++;
  }
  while(rightSide[k] != NULL){
    k++;
  }
  return piped;
}

int sh( int argc, char **argv, char **envp )
{
  /*parameters*/
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline;
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args;
  int uid, i, status, argsct, go = 1, errno, histct=0, background, noclobber;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;
  /*our history of commands executed list*/
  struct command_node *historylist=NULL;
  struct command_node *builtin = NULL;
  struct alias_node *aliases = NULL;
  struct inbox_node *inboxes = NULL;
  int warn_thrd_chk = 0; //flag to signal warn thread load already started
  int contains_pipe=0; //flag to signal there is a pipe to handle
  int first_pass = 1;

  /* Register the sig_chldHandler with sigaction */
  struct sigaction sa;
  sa.sa_handler = &sig_chldHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  if (sigaction(SIGCHLD, &sa, 0) == -1) {
    perror(0);
    exit(1);
  }

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
  out with*/

  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  /*creates a list of built in commands for alias use later*/
  builtin = create_builtinlist();

  //watches for Ctrl+C, Ctrl+Z
  signal(SIGINT, sig_intHandler);
  signal(SIGTSTP, sig_stpHandler);

  /*initialize environment variable*/
  sh_setenv_args("OWD", get_pwd());

  args = calloc(MAXARGS, sizeof(char*));
  int j; /* used to clear args */
  noclobber = 0; /*initialized to off */

  while ( go )
  {
    j = 0;
    commandline = calloc(MAX_CANON, sizeof(char));
    while(args[j] != NULL) {
      args[j++] = NULL;
    }

    background = 0;

    /* print your prompt */
    printf("%s [%s]> ", prompt, get_pwd());
    sh_setenv_args("PWD",get_pwd());

    /* get command line and process & also checks for ctrl D*/
    if (fgets(commandline, MAX_CANON, stdin) == NULL) {
      printf("\nUse \"exit\" to leave mysh.\n");
      continue;
    }
    if(strcmp(commandline, "\n") == 0) {
      continue;
    }

    /*checks for the type of pipe and then flags*/
    if((strstr(commandline,"|")!=NULL)) {
      contains_pipe = 1; //contains a pipe
    }
    if((strstr(commandline,"|&")!=NULL)){
      contains_pipe = 2;
    }

    strtok(commandline, "\n");

    /*add to our history list*/
    historylist = add_command(historylist, commandline);
    histct++;

    /*parsing the command line, returns the number of arguments*/
    argsct = parseLine(commandline, args);

    /* check aliases */
    if(aliases !=  NULL) {
      struct alias_node *curr_alias = aliases;
      while(curr_alias != NULL) {
        /* if args[0] matches an aliased command */
        if(strcmp(curr_alias->alias, args[0]) == 0) {
          strcpy(commandline, curr_alias->command);
          argsct = parseLine(commandline, args); /*parse the aliased command */
          break;
        }
        curr_alias = curr_alias->next;
      }
    }

    /*check for all the different commands*/
    if ((strcmp("which", args[0]))==0) { /*runs which, shows paths*/
      printf("Executing built-in which\n");
      if (argsct<2) { /*must take in at least one arg*/
        errno = EINVAL;
        perror("which");
      }
      else if(argsct==2) { /*runs which on arg*/
        printf("%s\n", which(args[1], pathlist));
      }
      else {
        int i=1;
        //if given more than one path to print
        while(i<argsct){
          printf("%s\n", which(args[i], pathlist));
          i++;
        }
      }
      continue;
    }
    else if((strcmp("where", args[0]))==0) { /*executes where*/
      printf("Executing built-in where\n");
      if(argsct < 2) { /*can only run when passed in with an arg*/
        errno = EINVAL;
        perror("where");
      }
      else { /*runs where with arg*/
        int i=1;
        while(i < argsct) {
          where(args[i], aliases, builtin, pathlist);
          i++;
        }
      }
      continue;
    }
    else if((strcmp("cd", args[0]))==0) { /*allows the user to change directory*/
      printf("Executing built-in cd\n");
      sh_setenv_args("TMP_DIR", get_pwd());


      //if no arguments cd home
      if(argsct < 2) { /*no args, cd to home*/
        sh_setenv_args("OWD",get_pwd());
        cd(getenv("HOME"));
      }
      else if((strcmp(args[1],"-"))==0) { /*cd - and goes to the old working directory*/
        sh_setenv_args("TMP_DIR", get_pwd());
        if (cd(getenv("OWD")) == 0) {
          sh_setenv_args("OWD", getenv("TMP_DIR"));
        }
      }
      else if(argsct==2){ /*cd to arg passed in by user*/
        if(cd(args[1]) == 0) {
          sh_setenv_args("OWD",get_pwd());
        }
      }
      else { /*handles error*/
        errno=E2BIG;
        perror("cd");
      }
      sh_setenv_args("PWD",get_pwd());
      continue;
    }
    else if((strcmp("pwd", args[0]))==0) { /*runs pwd*/
      printf("Executing built-in pwd\n");
      printf("%s\n", get_pwd()); /*runs pwd on current directory*/
      continue;
    }
    else if((strcmp("pid", args[0]))==0) { /*gets and prints shell pid*/
      printf("Executing built-in pid\n");
      print_pid();
      continue;
    }
    else if((strcmp("kill", args[0]))==0) { /*kills specified process*/
      printf("Executing built-in kill\n");
      if(argsct>1) {
        sh_kill(args,argsct);
      }
      else { /*can only run when given args*/
        errno=EINVAL;
        perror("kill");
      }
      continue;
    }
    else if((strcmp("history", args[0]))==0) { /*lists history*/
      printf("Executing built-in history\n");
      //we check if there was no args passed
      if(argsct < 2) {
        history(-1,histct,historylist);
      }
      else { /*lists history according to given num*/
        int incount = atoi(args[1]);
        history(incount,histct,historylist);
      }
      continue;
    }
    else if((strcmp("printenv", args[0]))==0) { /*prints environment*/
      printf("Executing built-in printenv\n");
      //if proper amount of arguments
      if (argsct < 2) { /*prints everything*/
        printenv_whole();
      }
      else if(argsct==2) { /*prints specificed env variable*/
        printenv_arg(args[1]);
      }
      //if passed in more than 2 arguments
      else {
        errno = E2BIG;
        perror("printenv");
      }
      continue;
    }
    else if((strcmp("setenv", args[0]))==0) { /*sets environment variable*/
      printf("Executing built-in setenv\n");
      /*if not passed in any arguments, printenv whole environment*/
      if(argsct<2) {
        printenv_whole();
      }
      /*create the environment with argument passed and blank value*/
      else if(argsct==2) {
        sh_setenv(args[1]);
        /*PATH special case, must free path list before setenv on PATH*/
        if(strcmp(args[1],"PATH")==0) {
          free_pathlist(pathlist);
          pathlist = get_path();
        }
      }
      /*create environment and set the third arg given to the environment variable*/
      else if(argsct==3) {
        sh_setenv_args(args[1], args[2]);
        /*PATH special case, must free path list before setenv on PATH*/
        if(strcmp(args[1],"PATH")==0) {
          free_pathlist(pathlist);
          pathlist = get_path();
        }
      }
      else { /*error handler*/
        errno = E2BIG;
        perror("setenv");
      }
      continue;
    }
    else if((strcmp("list", args[0]))==0) { /*lists files*/
      printf("Executing built-in list\n");
      /*list everything if not args*/
      if (argsct < 2) {
        char cwd[MAX_PATH];
        getcwd(cwd,sizeof(cwd));
        list(cwd);
      }
      else { /*lists for every folder passed in by user*/
        int i=1;
        while(args[i]!=NULL) {
          list(args[i]);
          i++;
        }
      }
      continue;
    }
    else if (strcmp("alias", args[0]) == 0) {
      printf("Executing built-in alias\n");
      if (argsct == 1) { /*list aliases if no arg given*/
        print_list(aliases);
        continue;
      }
      if (argsct >= 3) { /*sets an alias with given args*/
        char* new_command = calloc(MAX_CANON, sizeof(char));
        int i=2;
        while(args[i]!=NULL) {
          strcat(new_command, args[i]);
          strcat(new_command, " ");
          i++;
        }
        /*add alias*/
        aliases = add_alias(aliases, args[1], new_command);
        continue;
      }
    }
    else if((strcmp("prompt", args[0]))==0) { /*allows user to change prompt*/
      printf("Executing built-in prompt\n");
      if(argsct == 1) { /*asks user to provide prompt*/
        strcpy(prompt, get_prompt());
      }
      else if (argsct == 2) { /*sets prompt with given arg*/
        strcpy(prompt, args[1]);
      }
      else { /*can only pass in one arg to prompt*/
        errno = E2BIG;
        perror("prompt");
      }
      continue;
    }
    else if((strcmp("watchmail", args[0]))==0)  {
      printf("Executing built-in watchmail\n");
      if(argsct == 1) { /*asks user to provide prompt*/
        errno = EINVAL;
        fprintf(stderr,"%s: too few arguments.", args[0]);
        continue;
      }
      else if(argsct == 2) { /*when filename is only passed in parameter */
        inboxes = add_watchmail(inboxes, args[1]);
        /*print_inboxes(inboxes); */
        continue;
      }
      else if(argsct == 3) { /* when filename and 'off' are given */
        if(strcmp(args[2], "off") == 0) {
          inboxes = remove_watchmail(inboxes, args[1]);
          /* print_inboxes(inboxes); /* for testing purposes */
          continue;
        }
        else {
          errno = EINVAL;
          perror("watchmail");
          continue;
        }
      }
      else { /*can only pass in one arg to prompt*/
        errno = E2BIG;
        perror("watchmail");
      }
      continue;
    }
    else if((strcmp("warnload", args[0]))==0) {
      printf("Executing built-in %s \n", args[0]);

      double temp=0.0;
      if (argsct == 2) {
        temp = atof(args[1]);
        warn_lvl = temp; //assigns the user input to the global warning level
        if (warn_lvl == 0.0) { //checks if the warn level is zero
          warn_thrd_chk = 0; //reset thread check back to zero
          printf("exit thread %f : %f ", warn_lvl, temp);
        }
        else { //if the warn level is valid
          if (warn_thrd_chk==0) { //can i actually pass in showload to the last arg?
            create_warnthread();
            // pthread_create(pid, NULL, warn_thread, "WARNING THREAD");
            warn_thrd_chk = 1; //switch check to 1, prevents from creating another thread
          }
        }
      }
      else if(argsct < 2 || argsct > 2) {
        errno = EINVAL;
        perror("warnload");
      }
      continue;
    }
    else if((strcmp("watchuser", args[0]))==0) { //check if we can access the list from here
      pthread_mutex_t lock;
      char *temp = malloc(200);
      strcpy(temp,args[1]); //the word to copy over
      if (pthread_mutex_init(&lock, NULL) != 0)
      {
          printf("\n mutex init failed\n");
          return 1;
      }

      if (watchuser_switch==0) { //creates the new thread for watching users
        create_watchthread();
      }

      //adds and removes from our watchuser list
      if (argsct==2) { //add user here
          pthread_mutex_lock(&lock);
          userlist = add_command(userlist,temp); //add to userlist
          pthread_mutex_unlock(&lock);
          // print_clist(userlist);
      }
      else if (argsct==3 && (strcmp(args[2], "off")==0)) { //remove a user from our watch list
          pthread_mutex_lock(&lock);
          userlist = remove_node(userlist, temp);
          pthread_mutex_unlock(&lock);
          // print_clist(userlist);
      }
      else {
          errno = EINVAL;
          perror("watchuser");
      }
      free(temp); //free memory for temp
      continue;
    }
    else if(strcmp(args[0], "noclobber") == 0) {
      if (argsct == 1) {
        noclobber = (noclobber == 0);
        printf("noclobber set to %d\n", noclobber);
      } else {
        errno = E2BIG;
        perror("noclobber");
      }
      continue;
    }

    else if((strcmp("exit", args[0]))==0) { /*allows user to exit shell*/
      printf("Executing built-in exit\n");
      exit_program();
    }
    else {
      pid_t pid;
      char *p = args[0];
      /* checks to see if the last argument is '&' */
      background = (strcmp(args[argsct-1], "&") == 0);

      /* check for command in path list */
      if (access(args[0], F_OK) == -1) {
        p = which(args[0], pathlist);
      }

      if(strcmp(p, "Command not found") == 0) { /* if p is not in the path */
        perror("Command not found"); /* print error and continue */
        continue;
      }
      else if (contains_pipe>0 && contains_pipe<3) { //found pipe
        printf("Executing pipe \n");
        //parse the command line into two vectors
        char **c1_left, **c2_right;
        c1_left = calloc(MAXARGS, sizeof(char*));
        c2_right = calloc(MAXARGS, sizeof(char*));

        sep_commands(c2_right, c1_left, args, commandline);
        pipe_processes(c1_left, c2_right, contains_pipe); //take in third arg to flag & or not

        contains_pipe = 0; //reset the flag
        continue;
      }
      else if(contains_pipe==0) { //no pipe,
        pid=fork(); /* fork parent, create new child */
        if(pid < 0) {
          perror("Fork");
          exit(1);
        }
        else if(pid == 0) { /* run child */
            printf("Executing %s\n",p);

            if (argsct > 2) {
              if(strcmp(args[argsct-2], ">") == 0) {
                if(redirect_stdout(args[argsct-1], noclobber)) {
                  continue;
                }
                args[argsct-2] = NULL; /*remove redirect from args */
                args[argsct-1] = NULL; /*remove filename from args */
              }
              else if(strcmp(args[argsct-2], ">&") == 0) {
                if(redirect_stderr(args[argsct-1], noclobber)) {
                  continue;
                }
                args[argsct-2] = NULL; /*remove redirect from args */
                args[argsct-1] = NULL; /*remove filename from args */
              }
              else if(strcmp(args[argsct-2], ">>") == 0) {
                if(redirect_stdout_append(args[argsct-1], noclobber)) {
                  continue;
                }
                args[argsct-2] = NULL; /*remove redirect from args */
                args[argsct-1] = NULL; /*remove filename from args */
              }
              else if(strcmp(args[argsct-2], ">>&") == 0) {
                if(redirect_stderr_append(args[argsct-1], noclobber)) {
                  continue;
                }
                args[argsct-2] = NULL; /*remove redirect from args */
                args[argsct-1] = NULL; /*remove filename from args */
              }
              else if(strcmp(args[argsct-2], "<") == 0) {
                redirect_stdin(args[argsct-1]);
                args[argsct-2] = NULL; /*remove redirect from args */
                args[argsct-1] = NULL; /*remove filename from args */
              }
            }

          if(background) {
            args[argsct-1] = NULL; /*remove '&' from args */
          }
          if(execvp(p,args)) {
            perror(p);
            exit(1);
          }
        }
        else if(pid > 0 && !background){ /* wait parent */
          int childStat;
          waitpid(pid,&childStat,0); /* hold parent until child finishes */
        }
      }
    }
  }
  return 0;
} /* sh() */

//gets and prints the process id
void print_pid() {
  printf("%s %d\n", "PID:" , getpid());
}

/*print whole environment*/
void printenv_whole() {
  extern char **environ;
  int i;
  for(i = 0; environ[i] != NULL; i++) {
    printf("%s\n",environ[i]);
  }
}

/*prints environment of given argument*/
void printenv_arg(char *arg) {
  printf("%s\n", getenv(arg));
}

/*asks the user for a prompt and changes prompt*/
char *get_prompt() {
  char *new_prompt = calloc(PROMPTMAX, sizeof(char));
  printf("Enter a new prompt: ");
  fgets(new_prompt, PROMPTMAX, stdin);
  strtok(new_prompt, "\n");
  return new_prompt;
}

//exit the program
void exit_program() {
  exit(0);
}
