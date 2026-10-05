#include "input/completion.h"
#include "input/input.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int handle_backspace(ssize_t *num_char) {
  if (*num_char > 0) {
    (*num_char)--;
    if (delete_from_userin(1) == -1) {
      return -1;
    }
  }
  return 0;
}

int handle_tab(ssize_t *num_char, char **cmd) {

  char *filepath;
  size_t to_delete;

  (*cmd)[*num_char] = '\0';

  if (get_autocomplete_filepath(*cmd, &filepath, &to_delete) != 0) {
    return -1;
  }

  delete_from_userin(to_delete);
  *num_char -= to_delete;
  // (*cmd)[num_char] = '\0';

  size_t to_write = strlen(filepath);

  char *null_adress = (*cmd) + *num_char;

  memcpy(null_adress, filepath, to_write + 1);
  *num_char += to_write;

  write(STDOUT_FILENO, filepath, to_write);

  return 0;
}
