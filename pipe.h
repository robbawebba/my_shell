#pragma once

// char *test1[] = { "echo", "hello world", 0};
// char *test2[] = { "wc", 0};

// int main_pipe();

/* run the first part of the pipeline, cmd1 */
void src(int pfd[], char ** cmd1, int contains_pipe);

/* run the second part of the pipeline, cmd2 */
void rundest(int pfd[], char ** cmd2);

/*pipes the processes and checks for which type of pipe*/
int pipe_processes(char ** cmd1, char ** cmd2, int contains_pipe);
