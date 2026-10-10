#define DS_C_IMPLEMENTATION
#include "builtins.h"
#include "ds_string.h"
#include "executor.h"
#include "input/input.h"
#include "parser.h"
#include <libds_c.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

static int set_raw_terminal(struct termios *old_attr, struct termios *new_attr);
static int set_cannonical_terminal(struct termios *old_attr);

int main(void) {

  struct termios old_attr, new_attr;

  if (set_raw_terminal(&old_attr, &new_attr) != 0) {
    return -1;
  }

  // char *cmd = NULL;
  ds_string cmd;

  int argc = 0;
  char **argv = NULL;
  char delim = ' ';
  ssize_t cmdReadResult;

  while (1) {
    cmdReadResult = get_cmd_noncannonical(&cmd);

    if (cmdReadResult == -1) {
      reset(&argc, &cmd, &argv);
      continue;
    }

    if (tokenize(&argc, &argv, &cmd, delim) != 0) {
      reset(&argc, &cmd, &argv);
      continue;
    }

    write(STDOUT_FILENO, "\n", 1);
    if (argc == 0) {
      reset(&argc, &cmd, &argv);
      continue;
    }

    if (strcmp(argv[0], "exit") == 0) {
      reset(&argc, &cmd, &argv);
      break;
    }

    bool handled = handleBuiltins(argv, argc);

    if (!handled) {
      execute(&argv);
    }

    reset(&argc, &cmd, &argv);
  }

  if (set_cannonical_terminal(&old_attr) != 0) {
    return -1;
  }

  return 0;
}

static int set_raw_terminal(struct termios *old_attr,
                            struct termios *new_attr) {

  if (tcgetattr(STDIN_FILENO, old_attr) != 0) {
    perror("Fail in tcgetattr(). Could not fetch current terminal attributes");
    return -1;
  }

  *new_attr = *old_attr;
  new_attr->c_lflag &= ~ICANON;

  new_attr->c_lflag &= ~ECHO;

  if (tcsetattr(STDIN_FILENO, TCSANOW, new_attr) != 0) {
    perror("Fail in tcsetattr(). Could not go to cannonical mode");
    return -1;
  }

  return 0;
}

static int set_cannonical_terminal(struct termios *old_attr) {

  if (tcsetattr(STDIN_FILENO, TCSANOW, old_attr) != 0) {
    perror("Fail in tcsetattr(). Could not return to cannonical mode");
    return -1;
  }

  return 0;
}
