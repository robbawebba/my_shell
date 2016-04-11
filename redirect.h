#pragma once

int redirect_stdin(char *filename);
int redirect_stdout(char *filename, int noclobber);
int redirect_stderr(char *filename, int noclobber);
int redirect_stdout_append(char *filename, int noclobber);
int redirect_stderr_append(char *filename, int noclobber);

void reset_stdout();
void reset_stderr();
void reset_stdin();
