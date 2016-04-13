# mysh
A custom-made Unix shell.

## Objectives

The objectives of this project was to learn how a Unix shell works, to write a simple shell, to create processes, to handle signals, to use various Unix system and C library calls, and to become a better C programmer.

## Overview

This shell is implemented with a loop that prints a prompt (consisting of a "prefix," which is initially a blank (space), followed by ` [cwd]> `, where cwd is the "current working directory," and will execute commands until the `exit` command is entered. In the loop, we check if the command is one of the built-in commands (listed below) or a program in the search paths.

*This shell only works on **Unix systems** (Mac/Linux) and has **only been tested on OSX 10.11.4 and Ubuntu 14.04 LTS.** *

## How to Use this Shell
1. Clone or download/extract this repository to the location of your choice
1. Navigate to the directory with the shell files with `cd`
2. run `make clean`
3. run `make`
4. run `./mysh`

## Built-in commands and features
* **exit** - Terminates the shell.
* **which** - Find the first instance of the supplied command in the PATH variable. This instance is what is used for executing external commands.
* **where** - Reports all known instances of command in the PATH variable
* **cd** - Changes directory to directory given.
  * With no arguments, changes directory to the home directory.
  * With a '-' as the only argument, changes directory to directory previously in
  * With one argument, changes the directory to the given path.
* **pwd** - prints the current working directory
* **list** - A built-in equivalent to `ls`
  * With no arguments, lists the files in the current directory one per line.
  * With arguments, lists the files in each directory given as an argument, with a blank line then the name of the directory followed by a : before the list of files in that directory
* **pid** - prints the pid of the shell
* **kill** - When given just a pid sends a `SIGTERM` to it with kill(2). When given a signal number (with a - in front of it) sends that signal to the pid. (e.g., `kill 5678`, `kill -9 5678`).
* **prompt** - When ran with no arguments, prompts for a new prompt prefix string. When given an argument make that the new prompt prefix.For instance, let's assume cwd is /usa/robertweber95:
```
[/usa/robertweber95]> prompt CISC361
CISC361 [/usa/robertweber95]>
CISC361 [/usa/robertweber95]> cd 361
CISC361 [/usa/robertweber95/361]> prompt YES
YES [/usa/robertweber95/361]> prompt
input prompt prefix: hello
hello [/usa/robertweber95/361]>
```
* **printenv**
  * When ran with no arguments prints the whole environment.
  * When ran with one argument, prints the value of the given environment variable.
* **alias**
  * When ran with no arguments prints the aliases the shell knows about.
  * When ran with arguments it should install an alias into the alias table the shell knows about.
* **history** - Prints the last 10 commands executed when ran with no     options.
  * When ran with a numerical argument, changes the number of commands to list to that number.
* **setenv**
  * When ran with no arguments, prints the whole environment, the same as printenv.
  * When ran with one argument, sets the value of the argument as an empty environment variable.
  * When ran with two arguments, the second one is the value of the first.
* **Job Backgrounding using `&`** - When the `&` is the last argument supplied in a command, the given command is executed concurrently as a background job along with the shell, rather than having the shell wait for the process to complete. It is possible that several backgrounded jobs could be running concurrently at the same time. *This only works for external commands.*
* **watchmail** - This command takes as the first argument a name of a file, which must already exist to watch for an increase in it's file size. It can take an optional second argument of "off" to turn off of watching of mail for that file:
```
mysh >> watchmail mbox
mysh >> echo hello > mbox
  [You've got mail in mbox on DD/MM/YYYY at hh:mm::ss]
mysh >> watchmail mbox off
```
* **File Redirections** - The following file redirections will work:
  *  `>` redirects standard output of command on left to file on right (i.e. `command > file`).
  * `>&` redirects standard output and standard error of command on left to file on right (i.e. `command >& file`).
  * `>>` redirects standard output of command on left to file on right (i.e. `command >> file`), appending to the file.
  * `>>&` redirects standard output and standard error of command on left to file on right (i.e. `command >>& file`), appending to the file.
  `<` redirects standard input of command on left to come from file on the right (i.e. `command < file`).
* **noclobber** - Affects how file redirections handle file overwriting and creation. Noclobber prevents accidental overwriting of existing files. Noclobber defaults to 'off'.
  * when noclobber is off, > and >& will overwrite existing files and >> and >>& will create the file if it doesn't exist.
  * When noclobber is on, the shell refuses to overwrite existing files and refuses to create a file when appending to a non-existing file.
  ```
  [mudskipper6:/usa/cshen/public_html/361/Proj_3 216] tcsh
  mudskipper6[31] [~/public_html/361/Proj_3/]> touch out-file
  mudskipper6[32] [~/public_html/361/Proj_3/]> noclobber
  mudskipper6[33] [~/public_html/361/Proj_3/]> echo hello > out-file
  out-file: File exists.
  mudskipper6[34] [~/public_html/361/Proj_3/]> noclobber
  mudskipper6[35] [~/public_html/361/Proj_3/]> echo hello > out-file
  mudskipper6[36] [~/public_html/361/Proj_3/]> more out-file
  hello
  mudskipper6[37] [~/public_html/361/Proj_3/]>
  ```
* **interprocess communication (IPC)**. This is adding support for `|` and `|&`. The `|` operator pipes standard output of the command on the left to standard input of the command on the right. (i.e. `command1 | command2`). For `|&`, standard error piped as well as standard output. This only works when the command on the right is an external command, however the command on the left could be built-in or external.
  ```
  [yankees6.acad.ece.udel.edu:/usa/cshen 132] tcsh
  yankees6[29] [~/]> which echo
  usr/bin/echo
  yankees6[30] [~/]> echo hello world | wc
         1       2      12
  yankees6[31] [~/]>
  yankees6[34] [~/361/2016/apue.3e/intro/]>
  yankees6[34] [~/361/2016/apue.3e/intro/]> ls X
  X: No such file or directory
  yankees6[35] [~/361/2016/apue.3e/intro/]> ls X | wc
  X: No such file or directory
         0       0       0
  yankees6[36] [~/361/2016/apue.3e/intro/]> ls X |& wc
         1       6      29
  yankees6[37] [~/361/2016/apue.3e/intro/]>
  ```
## Testing commands
Below is a list of some commands you can use to test the functionality of this shell.
```
[return]
Ctrl-D
Ctrl-Z
Ctrl-C
which					  ; test which
which ls
ls					    ; execute it
[return]
Ctrl-D					; make sure still work
Ctrl-Z
Ctrl-C
ls -l					  ; test passing arguments
ls -l -a /bin/ps
where					  ; test where
where ls		
/bin/ls -l -g   ; test absolutes and passing args
/bin/ls -l
blah					  ; try a command that doesn't exist
/blah					  ; an absolute path that doesn't exist
ls -l /blah
/bin				; try to execute a directory
/bin/ls -la /
file /bin/ls /bin/rm
which ls rm			; test multiple args
where ls rm
list					  ; test list
list / /usr/bin
cd 					    ; cd to home
pwd
cd /blah				; non-existant
cd /usr/bin /usr/ucb			; too many args
cd -					; should go back to shell dir
pwd
cd /bin
pwd
./ls /					; test more absolutes
../bin/ls /
history					; test history
history 15
history
history 5
history
pid					; get pid for later use
kill
kill pid-of-shell			; test default
kill -1 pid-of-shell			; test sending a signal, should kill
					                ; the shell, so restart a new one
prompt	    (and enter some prompt prefix)
prompt mysh
alias lst ls -l
alias
ls
lst
where lst
printenv PATH
printenv
setenv
setenv TEST
printenv TEST
setenv TEST testing
printenv TEST
setenv TEST testing more
setenv HOME /
cd
pwd
cd -
pwd
ls &
ls -l &
sleep 20 &
ls &          ; Execute before sleep ends
ps            ; call ps to see backgrounded sleep process
pid
tty
/bin/ps -lfu rweber
ls -l
rm -f mail1 mail2
touch mail1
watchmail mail1
echo hi >> mail1
echo HiThere > mail2
watchmail mail2
echo there >> mail1
echo Subject: >> mail2
cat mail1
cat mail2
watchmail mail1 off
echo bye >> mail1
echo bye >> mail2
grep hi < mail1
ls | fgrep .c
pid
/bin/ps -lfu rweber | grep defunct  ; Check for zombie processes
exit
```
