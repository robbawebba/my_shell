#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "redirect.h"

int redirect_stdin(char *filename) {
  int fid = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 00666);
  if(fid < 0) {
    perror(filename);
    return fid;
  }
  close(0);
  dup(fid);
  close(fid);
  return 0;
}

int redirect_stdout(char *filename, int noclobber) {
  int fid;
  if(noclobber) {
    fid = open(filename,O_WRONLY|O_CREAT|O_EXCL|O_TRUNC, 00666);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  else {
    fid = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 00666);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  close(1);
  dup(fid);
  close(fid);
  return 0;
}

int redirect_stderr(char *filename, int noclobber) {
  int fid;
  if(noclobber) {
    fid = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 00666);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  else {
    fid = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 00666);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  close(1);
  close(2);
  dup(fid);
  dup(fid);
  close(fid);
  return 0;
}

int redirect_stdout_append(char *filename, int noclobber) {
  int fid;
  if(noclobber) {
    fid = open(filename,O_WRONLY|O_APPEND);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  else {
    fid = open(filename,O_WRONLY|O_CREAT|O_APPEND, 00666);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  close(1);
  dup(fid);
  close(fid);
  return 0;
}

int redirect_stderr_append(char *filename, int noclobber) {
  int fid;
  if(noclobber) {
    fid = open(filename,O_WRONLY|O_APPEND);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  else {
    fid = open(filename,O_WRONLY|O_CREAT|O_APPEND, 00666);
    if(fid < 0) {
      perror(filename);
      return fid;
    }
  }
  close(1);
  close(2);
  dup(fid);
  dup(fid);
  close(fid);
  return 0;
}

void reset_stdin() {
  int fid = open("/dev/tty",O_RDONLY);
  close(0);
  dup(fid);
  close(fid);
}

void reset_stdout() {
  int fid = open("/dev/tty",O_WRONLY);
  close(1);
  dup(fid);
  close(fid);
}

void reset_stderr(){
  int fid = open("/dev/tty",O_WRONLY);
  close(2);
  dup(fid);
  close(fid);
}
