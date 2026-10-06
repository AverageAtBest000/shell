#include "ds_string.h"
#include "input/completion.h"
#include "input/input.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int handle_backspace(ds_string cmd) {
  if (cmd.length > 0) {
    if (delete_from_userin(1) == -1) {
      return -1;
    }
  }
  return 0;
}

int handle_tab(ds_string *cmd) {

  char *filepath;
  size_t to_delete;

  if (get_autocomplete_filepath(cmd, &filepath, &to_delete) != 0) {
    return -1;
  }

  delete_from_userin(to_delete);

  ds_string_append_c_str(cmd, filepath);

  size_t to_write = strlen(filepath);
  write(STDOUT_FILENO, filepath, to_write);

  return 0;
}
