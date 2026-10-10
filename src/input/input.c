#include "ds_string.h"
#include "input/handlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int delete_from_userin(int num_chars);

ssize_t get_cmd_cannonical(char **cmd) {

  printf("❯ ");

  size_t n = 10;
  ssize_t numchar = getline(cmd, &n, stdin);

  if (numchar == -1) {

    if (feof(stdin)) {
      return 0;
    } else {
      perror("Error in getline() function");
      return -1;
    }
  }

  if (numchar > 0 && (*cmd)[numchar - 1] == '\n')
    (*cmd)[numchar - 1] = '\0';

  return numchar;
}

ssize_t get_cmd_noncannonical(ds_string *cmd) {

  printf("❯ ");
  fflush(stdout);

  ds_string_init(cmd, "");

  char ch;

  // read() will return a \r once the user hits Enter
  while (1) {

    if (read(STDIN_FILENO, &ch, 1) == -1) {
      perror("Error in read() operation");
      return -1;
    }

    if (ch == '\n' || ch == '\r') {
      break;
    }

    if (ch == '\b' || ch == 8 || ch == 127) {
      handle_backspace(cmd);
      continue;
    }

    if ((ch == '\t' || ch == 9) && cmd->length != 0) {
      handle_tab(cmd);
      continue;
    }

    write(STDOUT_FILENO, &ch, 1);
    ds_string_append_char(cmd, ch);
  }
  return cmd->length;
}

int delete_from_userin(int num_chars) {
  while (num_chars-- > 0) {
    if (write(STDOUT_FILENO, "\033[D \033[D", 7) == -1) {
      perror("Error in write() operation");
      return -1;
    }
  }

  return 0;
}

void reset(int *argc, ds_string *cmd, char ***argv) {
  for (int i = 0; i < *argc; i++)
    free((*argv)[i]);
  *argc = 0;

  ds_string_deinit(cmd);

  free(*argv);
  *argv = NULL;
}
