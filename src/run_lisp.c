/*
 * File: run_lisp.c
 * ----------------
 * Presents the implementation of the lisp file runner
 */

#include <run_lisp.h>
#include <lisp.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int run(const char const* lispProgramPath) {

  // Error checking
  if (access(lispProgramPath, R_OK) != 0) {
    if (errno == ENOENT) fprintf(stderr, "No such file: %s\n", lispProgramPath);
    if (errno == EACCES) fprintf(stderr, "Permissions denied: %s\n", lispProgramPath);
    return errno;
  }

  FILE* fd = fopen(lispProgramPath, O_RDONLY);
  while (!feof(fd)) eval(read(fd, NULL, NULL));
}