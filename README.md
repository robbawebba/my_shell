# my_shell

## Objectives

The objectives of this project was to learn how a Unix shell works, to write a simple shell, to create processes, to handle signals, to use various Unix system and C library calls, and to become a better C programmer. 

## Overview

This shell is implemented with a loop that prints a prompt (consisting of a "prefix," which is initially a blank (space), followed by ` [cwd]> `, where cwd is the "current working directory," and will execute commands until the `exit` command is entered. In the loop, we check if the command is one of the built-in commands (listed below) or a program in the search paths.


To execute:
1. cd into the folder
2. run 'make clean'
3. run 'make'
4. run './mysh'
5. use the terminal as is
